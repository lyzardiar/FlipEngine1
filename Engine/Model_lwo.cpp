#include "Model_lwo.h"

/*
=====================================================================
RevBytesSwap

Reverses byte order in place.

INPUTS
   bp       bytes to reverse
   elsize   size of the underlying data type
   elcount  number of elements to swap

RESULTS
   Reverses the byte order in each of elcount elements.
===================================================================== */
void RevBytesSwap( void *bp, int elsize, int elcount ) {
	register unsigned char *p, *q;

	p = ( unsigned char * ) bp;

	if ( elsize == 2 ) {
		q = p + 1;
		while ( elcount-- ) {
			*p ^= *q;
			*q ^= *p;
			*p ^= *q;
			p += 2;
			q += 2;
		}
		return;
	}

	while ( elcount-- ) {
		q = p + elsize - 1;
		while ( p < q ) {
			*p ^= *q;
			*q ^= *p;
			*p ^= *q;
			++p;
			--q;
		}
		p += elsize >> 1;
	}
}

lwObject *lwGetObject( const char *filename)
{
	lfFile file;
	
	if (!file.Open(filename))
	{
		return NULL;
	}
	
	unsigned int id = file.ReadUnsignedInt();
	RevBytesSwap(&id, 4, 1);
	unsigned int formszie = file.ReadUnsignedInt();
	RevBytesSwap(&formszie, 4, 1);
	unsigned int type = file.ReadUnsignedInt();
	RevBytesSwap(&type, 4, 1);

	if ( id != ID_FORM ) 
		return NULL;

	if ( type != ID_LWO2 ) 
	{
		if ( type == ID_LWOB )
		{
		}
			//return lwGetObject5( filename );
		else
			return NULL;
	}

	/* get the first chunk header */

	id = file.ReadUnsignedInt();
	unsigned int cksize = file.ReadUnsignedInt();

	//while ( 1 ) {
	//	cksize += cksize & 1;

	//	switch ( id )
	//	{
	//	case ID_LAYR:
	//		if ( object->nlayers > 0 ) {
	//			layer = (lwLayer*)Mem_ClearedAlloc( sizeof( lwLayer ) );
	//			if ( !layer ) goto Fail;
	//			lwListAdd( (void**)&object->layer, layer );
	//		}
	//		object->nlayers++;

	//		set_flen( 0 );
	//		layer->index = getU2( fp );
	//		layer->flags = getU2( fp );
	//		layer->pivot[ 0 ] = getF4( fp );
	//		layer->pivot[ 1 ] = getF4( fp );
	//		layer->pivot[ 2 ] = getF4( fp );
	//		layer->name = getS0( fp );

	//		rlen = get_flen();
	//		if ( rlen < 0 || rlen > cksize ) goto Fail;
	//		if ( rlen <= cksize - 2 )
	//			layer->parent = getU2( fp );
	//		rlen = get_flen();
	//		if ( rlen < cksize )
	//			fp->Seek( cksize - rlen, FS_SEEK_CUR );
	//		break;

	//	case ID_PNTS:
	//		if ( !lwGetPoints( fp, cksize, &layer->point ))
	//			goto Fail;
	//		break;

	//	case ID_POLS:
	//		if ( !lwGetPolygons( fp, cksize, &layer->polygon,
	//			layer->point.offset ))
	//			goto Fail;
	//		break;

	//	case ID_VMAP:
	//	case ID_VMAD:
	//		node = ( lwNode * ) lwGetVMap( fp, cksize, layer->point.offset,
	//			layer->polygon.offset, id == ID_VMAD );
	//		if ( !node ) goto Fail;
	//		lwListAdd( (void**)&layer->vmap, node );
	//		layer->nvmaps++;
	//		break;

	//	case ID_PTAG:
	//		if ( !lwGetPolygonTags( fp, cksize, &object->taglist,
	//			&layer->polygon ))
	//			goto Fail;
	//		break;

	//	case ID_BBOX:
	//		set_flen( 0 );
	//		for ( i = 0; i < 6; i++ )
	//			layer->bbox[ i ] = getF4( fp );
	//		rlen = get_flen();
	//		if ( rlen < 0 || rlen > cksize ) goto Fail;
	//		if ( rlen < cksize )
	//			fp->Seek( cksize - rlen, FS_SEEK_CUR );
	//		break;

	//	case ID_TAGS:
	//		if ( !lwGetTags( fp, cksize, &object->taglist ))
	//			goto Fail;
	//		break;

	//	case ID_ENVL:
	//		node = ( lwNode * ) lwGetEnvelope( fp, cksize );
	//		if ( !node ) goto Fail;
	//		lwListAdd( (void**)&object->env, node );
	//		object->nenvs++;
	//		break;

	//	case ID_CLIP:
	//		node = ( lwNode * ) lwGetClip( fp, cksize );
	//		if ( !node ) goto Fail;
	//		lwListAdd( (void**)&object->clip, node );
	//		object->nclips++;
	//		break;

	//	case ID_SURF:
	//		node = ( lwNode * ) lwGetSurface( fp, cksize );
	//		if ( !node ) goto Fail;
	//		lwListAdd( (void**)&object->surf, node );
	//		object->nsurfs++;
	//		break;

	//	case ID_DESC:
	//	case ID_TEXT:
	//	case ID_ICON:
	//	default:
	//		fp->Seek( cksize, FS_SEEK_CUR );
	//		break;
	//	}

	//	/* end of the file? */

	//	if ( formsize <= fp->Tell() - 8 ) break;

	//	/* get the next chunk header */

	//	set_flen( 0 );
	//	id = getU4( fp );
	//	cksize = getU4( fp );
	//	if ( 8 != get_flen() ) goto Fail;
	//}
}