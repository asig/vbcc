	IFND	GRAPHICS_LAYERS_I
GRAPHICS_LAYERS_I	SET	1
	IFND	EXEC_SEMAPHORES_I
	include	"exec/semaphores.i"
	ENDC
	IFND	EXEC_LISTS_I
	include	"exec/lists.i"
	ENDC
LAYERSIMPLE	equ	1
LAYERSMART	equ	2
LAYERSUPER	equ	4
LAYERUPDATING	equ	$10
LAYERBACKDROP	equ	$40
LAYERREFRESH	equ	$80
LAYER_CLIPRECTS_LOST	equ	$100
LMN_REGION	equ	-1
	STRUCTURE	Layer_Info,0
	APTR	li_top_layer
	APTR	li_check_lp
	APTR	li_obs
	STRUCT	li_FreeClipRects,MLH_SIZE
	STRUCT	li_Lock,SS_SIZE
	STRUCT	li_gs_Head,LH_SIZE
	LONG	li_long_reserved
	WORD	li_Flags
	BYTE	li_fatten_count
	BYTE	li_LockLayersCount
	WORD	li_LayerInfo_extra_size
	APTR	li_blitbuff
	APTR	li_LayerInfo_extra
	LABEL	li_SIZEOF
NEWLAYERINFO_CALLED	equ	1
ALERTLAYERSNOMEM	equ	$83010000
	ENDC
