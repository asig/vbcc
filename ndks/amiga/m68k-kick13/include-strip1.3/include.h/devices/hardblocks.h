#ifndef	DEVICES_HARDBLOCKS_H
#define	DEVICES_HARDBLOCKS_H
struct	RigidDiskBlock	{
ULONG	rdb_ID;
ULONG	rdb_SummedLongs;
LONG	rdb_ChkSum;
ULONG	rdb_HostID;
ULONG	rdb_BlockBytes;
ULONG	rdb_Flags;
ULONG	rdb_BadBlockList;
ULONG	rdb_PartitionList;
ULONG	rdb_FileSysHeaderList;
ULONG	rdb_DriveInit;
ULONG	rdb_Reserved1[6];
ULONG	rdb_Cylinders;
ULONG	rdb_Sectors;
ULONG	rdb_Heads;
ULONG	rdb_Interleave;
ULONG	rdb_Park;
ULONG	rdb_Reserved2[3];
ULONG	rdb_WritePreComp;
ULONG	rdb_ReducedWrite;
ULONG	rdb_StepRate;
ULONG	rdb_Reserved3[5];
ULONG	rdb_RDBBlocksLo;
ULONG	rdb_RDBBlocksHi;
ULONG	rdb_LoCylinder;
ULONG	rdb_HiCylinder;
ULONG	rdb_CylBlocks;
ULONG	rdb_AutoParkSeconds;
ULONG	rdb_Reserved4[2];
char	rdb_DiskVendor[8];
char	rdb_DiskProduct[16];
char	rdb_DiskRevision[4];
char	rdb_ControllerVendor[8];
char	rdb_ControllerProduct[16];
char	rdb_ControllerRevision[4];
ULONG	rdb_Reserved5[10];
};
#define	IDNAME_RIGIDDISK	(('R'<<24)|('D'<<16)|('S'<<8)|('K'))
#define	RDB_LOCATION_LIMIT	16
#define	RDBFB_LAST	0
#define	RDBFF_LAST	0x01L
#define	RDBFB_LASTLUN	1
#define	RDBFF_LASTLUN	0x02L
#define	RDBFB_LASTTID	2
#define	RDBFF_LASTTID	0x04L
#define	RDBFB_NORESELECT	3
#define	RDBFF_NORESELECT	0x08L
#define	RDBFB_DISKID	4
#define	RDBFF_DISKID	0x10L
#define	RDBFB_CTRLRID	5
#define	RDBFF_CTRLRID	0x20L
struct	BadBlockEntry	{
ULONG	bbe_BadBlock;
ULONG	bbe_GoodBlock;
};
struct	BadBlockBlock	{
ULONG	bbb_ID;
ULONG	bbb_SummedLongs;
LONG	bbb_ChkSum;
ULONG	bbb_HostID;
ULONG	bbb_Next;
ULONG	bbb_Reserved;
struct	BadBlockEntry	bbb_BlockPairs[61];
};
#define	IDNAME_BADBLOCK	(('B'<<24)|('A'<<16)|('D'<<8)|('B'))
struct	PartitionBlock	{
ULONG	pb_ID;
ULONG	pb_SummedLongs;
LONG	pb_ChkSum;
ULONG	pb_HostID;
ULONG	pb_Next;
ULONG	pb_Flags;
ULONG	pb_Reserved1[2];
ULONG	pb_DevFlags;
UBYTE	pb_DriveName[32];
ULONG	pb_Reserved2[15];
ULONG	pb_Environment[17];
ULONG	pb_EReserved[15];
};
#define	IDNAME_PARTITION	(('P'<<24)|('A'<<16)|('R'<<8)|('T'))
#define	PBFB_BOOTABLE	0
#define	PBFF_BOOTABLE	1L
#define	PBFB_NOMOUNT	1
#define	PBFF_NOMOUNT	2L
struct	FileSysHeaderBlock	{
ULONG	fhb_ID;
ULONG	fhb_SummedLongs;
LONG	fhb_ChkSum;
ULONG	fhb_HostID;
ULONG	fhb_Next;
ULONG	fhb_Flags;
ULONG	fhb_Reserved1[2];
ULONG	fhb_DosType;
ULONG	fhb_Version;
ULONG	fhb_PatchFlags;
ULONG	fhb_Type;
ULONG	fhb_Task;
ULONG	fhb_Lock;
ULONG	fhb_Handler;
ULONG	fhb_StackSize;
LONG	fhb_Priority;
LONG	fhb_Startup;
LONG	fhb_SegListBlocks;
LONG	fhb_GlobalVec;
ULONG	fhb_Reserved2[23];
ULONG	fhb_Reserved3[21];
};
#define	IDNAME_FILESYSHEADER	(('F'<<24)|('S'<<16)|('H'<<8)|('D'))
struct	LoadSegBlock	{
ULONG	lsb_ID;
ULONG	lsb_SummedLongs;
LONG	lsb_ChkSum;
ULONG	lsb_HostID;
ULONG	lsb_Next;
ULONG	lsb_LoadData[123];
};
#define	IDNAME_LOADSEG	(('L'<<24)|('S'<<16)|('E'<<8)|('G'))
#endif
