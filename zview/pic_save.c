#include "general.h"
#include "pic_load.h"
#include "prefs.h"
#include "ztext.h"
#include "zvdi/color.h"
#include "zvdi/raster.h"
#include "zvdi/vdi.h"
#include "plugins.h"
#include "progress.h"
#include "pic_save.h"
#include "plugins/common/zvplugin.h"

static boolean encoder_init_done = FALSE;



static int16 setup_encoder ( IMGINFO in_info, IMGINFO out_info, DECDATA data)
{
	size_t	src_line_size, dst_line_size;
		
	data->IncXfx    = 0x10000uL;

	src_line_size   = ( in_info->width  + 15) * in_info->components;
	dst_line_size   = ( out_info->width + 15) * out_info->components;
	
	data->RowBuf = ( uint8*)malloc( src_line_size);
		
	if( data->RowBuf == NULL)
		return( 0);

	data->DthBuf = NULL;

	/* If the image is inverted like in iNTEL format, we must to decompress
	   the entire picture before to save it */
	if( in_info->orientation == DOWN_TO_UP)
	{	
		data->DstBuf = ( uint8*)malloc( dst_line_size * ( out_info->height + 1));
	}
	else 
		data->DstBuf = ( uint8*)malloc( dst_line_size);	

	if( data->DstBuf == NULL)
		return( 0);

	data->DthWidth 	= in_info->width;
	data->PixMask  	= ( 1 << in_info->planes) - 1;
	data->LnSize   	= out_info->width * out_info->components;

	switch( out_info->planes)
	{
		case 24:
			if( in_info->planes == 1 && in_info->components == 1)			
			{	
				data->Pixel[0] = 0xFFFFFF;
				data->Pixel[1] = 0x000000;
				raster = raster_24;
			}			
			else if( in_info->indexed_color)
			{	
				cnvpal_true( in_info, data);
				raster = raster_24;
			}
			else
				raster = ( in_info->components >= 3 ? dither_24 : gscale_24);

			break;

		default:
			return( 0);
	}

	return( 1);
}


/*==================================================================================*
 * read_img:																		*
 *				Read a image and fit the MFDB within IMAGE structur with the data.	*
 *----------------------------------------------------------------------------------*
 * img: 		The struct. with the MFDB to be fitted.								*
 * info:		The struct. with the line and dither buffer							*
 *----------------------------------------------------------------------------------*
 * returns: 	-																	*
 *==================================================================================*/
static void write_img( IMGINFO in_info, IMGINFO out_info, DECDATA data)
{
	int16  	y, img_h = in_info->height;
	uint8  	*dst	 = data->DstBuf;

	in_info->page_wanted = 0;

	if( in_info->orientation == UP_TO_DOWN)
	{	
		for( y = 0; y < img_h; y++)
		{
			if (curr_input_plugin)
				plugin_reader_read(&curr_input_plugin->c.slb, in_info, data->RowBuf);
			else
				ldg_funcs.decoder_read(in_info, data->RowBuf);
				
			( *raster)( data, dst);
	
			if (curr_output_plugin)
				plugin_encoder_write(&curr_output_plugin->c.slb, out_info, dst);
			else
				ldg_funcs.encoder_write(out_info, dst);

			if( show_write_progress_bar)
				win_progress(( int16)((( int32)y * 150L) / img_h));
		}
	}
	else /* inversed INTEL format */
	{	
		int16 h = ( img_h << 1);	/* image height * 2 . don't worry about this, is only for the progress bar */
		dst += data->LnSize * ( in_info->height - 1);

		/* First pass, we decode the entire image */
		for( y = 0; y < img_h; y++)
		{
			if (curr_input_plugin)
				plugin_reader_read(&curr_input_plugin->c.slb, in_info, data->RowBuf);
			else
				ldg_funcs.decoder_read(in_info, data->RowBuf);

			( *raster)( data, dst);
	
			dst -= data->LnSize;

			if( show_write_progress_bar)
				win_progress(( int16)((( int32)y * 75L) / img_h));
		}

		dst	 = data->DstBuf;

		/* second pass, we encode it */
		for( ; y < h; y++)
		{
			if (curr_output_plugin)
				plugin_encoder_write(&curr_output_plugin->c.slb, out_info, dst);
			else
				ldg_funcs.encoder_write(out_info, dst);

			dst += data->LnSize;

			if( show_write_progress_bar)
				win_progress(( int16)((( int32)y * 75L) / img_h));
		}
	}
}



static void exit_pic_save( IMGINFO in_info, IMGINFO out_info, DECDATA data)
{
	if( data->DthBuf) 
		free( data->DthBuf);

	if( data->DstBuf) 
		free( data->DstBuf);

	if( data->RowBuf) 
		free( data->RowBuf);

	if (encoder_init_done)
	{
		if (curr_output_plugin)
			plugin_encoder_quit(&curr_output_plugin->c.slb, out_info);
		else
			ldg_funcs.encoder_quit(out_info);
	}
	
	if (decoder_init_done)
	{
		if (curr_input_plugin)
			plugin_reader_quit(&curr_input_plugin->c.slb, in_info);
		else
			ldg_funcs.decoder_quit( in_info);
	}
	
	free( data);
	free( out_info);
	free( in_info);
	
	graf_mouse( ARROW, NULL);	

	encoder_init_done = FALSE;
	decoder_init_done = FALSE;
}


/*==================================================================================*
 * pic_load:																		*
 *				read a supported picture and fill a IMAGE structure.				*
 *----------------------------------------------------------------------------------*
 * file: 		 The file that will be read.										*
 * img: 		 The IMAGE struct. that will be filled. 			 				*
 *----------------------------------------------------------------------------------*
 * returns: 	'0' if error or picture not supported								*
 *==================================================================================*/
  
int16 pic_save( const char *in_file, const char *out_file)
{
	IMGINFO in_info;
	IMGINFO out_info;
	DECDATA data;

    graf_mouse( BUSYBEE, NULL);	

	in_info 	= ( img_info *) calloc(1, sizeof( img_info));
	out_info 	= ( img_info *) calloc(1, sizeof( img_info));	
	data 		= ( dec_data *) malloc( sizeof( dec_data));
	
	if ( !in_info || !out_info || !data)
	{
		free( data);
		free( out_info);
		free( in_info);

		graf_mouse( ARROW, NULL);			
		errshow(NULL, -ENOMEM);		
		return( 0);
	}

	data->DthBuf = NULL;
	data->DstBuf = NULL; 
	data->RowBuf = NULL;

	/* We initialise some variables needed by the codecs */
	in_info->background_color	= 0xFFFFFF;
	in_info->thumbnail			= FALSE;

	if(( decoder_init_done = get_pic_info( in_file, in_info)) == FALSE)
	{
		errshow(in_file, CANT_SAVE_IMG);
		exit_pic_save( in_info, out_info, data);
		graf_mouse( ARROW, NULL);
		return ( 0);
	}

	if( show_write_progress_bar)
		win_progress_begin( get_string( SAVE_TITLE));

	/* copy information from input's information to output's information struct */
	*out_info = *in_info;

	if (curr_output_plugin)
		encoder_init_done = plugin_encoder_init(&curr_output_plugin->c.slb, out_file, out_info);
	else
		encoder_init_done = ldg_funcs.encoder_init(out_file, out_info);
	if (encoder_init_done == FALSE)
	{
		errshow(out_file, CANT_SAVE_IMG);
		exit_pic_save( in_info, out_info, data);
		win_progress_end();
		graf_mouse( ARROW, NULL);
		return ( 0);
	}


	if( !setup_encoder( in_info, out_info, data))
	{
		errshow(NULL, -ENOMEM);	
		exit_pic_save( in_info, out_info, data);
		win_progress_end();
		graf_mouse( ARROW, NULL);
		return ( 0);
	}


	write_img( in_info, out_info, data);

	exit_pic_save( in_info, out_info, data);

	graf_mouse( ARROW, NULL);
	win_progress_end();

	return ( 1);
}
