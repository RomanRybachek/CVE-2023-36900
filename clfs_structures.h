typedef char CLFS_CLIENT_ID;
typedef ULONG CLFS_CONTAINER_ID;
typedef UCHAR CLFS_LOG_STATE;
typedef GUID CLFS_LOG_ID;
typedef UCHAR CLFS_USN;

#define CLIENT_SYMTBL_SIZE 11
#define CONTAINER_SYMTBL_SIZE 11
#define SHARED_SECURITY_SYMTBL_SIZE 11
#define MAX_CLIENTS_DEFAULT 124
#define MAX_CONTAINERS_DEFAULT 1024
#define CLFS_SECTOR_SIZE 0x200

typedef enum _CLFS_METADATA_BLOCK_TYPE
{
    ClfsMetaBlockControl,
    ClfsMetaBlockControlShadow,
    ClfsMetaBlockGeneral,
    ClfsMetaBlockGeneralShadow,
    ClfsMetaBlockScratch,
    ClfsMetaBlockScratchShadow
} CLFS_METADATA_BLOCK_TYPE, *PCLFS_METADATA_BLOCK_TYPE;

typedef enum _CLFS_TRUNCATE_STATE
{
    ClfsTruncateStateNone,
    ClfsTruncateStateModifyingStream,
    ClfsTruncateStateSavingOwner,
    ClfsTruncateStateModifyingOwner,
    ClfsTruncateStateSavingDiscardBlock,
    ClfsTruncateStateModifyingDiscardBlock
} CLFS_TRUNCATE_STATE, *PCLFS_TRUNCATE_STATE;

typedef struct _CLFS_SECTOR_CHANGE
{
    ULONG iSector;
    ULONG ulUnused;
    BYTE rgbSector[CLFS_SECTOR_SIZE];
} CLFS_SECTOR_CHANGE, *PCLFS_SECTOR_CHANGE;

typedef struct _CLFS_LSN{
    ULONGLONG Internal;
} CLFS_LSN, *PCLFS_LSN;

typedef struct _CLFS_NODE_ID
{
    ULONG   cType;                                      // CLFS node type.
    ULONG   cbNode;                                     // CLFS node size.
} CLFS_NODE_ID, *PCLFS_NODE_ID;

typedef struct _CLFS_LOG_BLOCK_HEADER
{
    UCHAR MajorVersion;
    UCHAR MinorVersion;
    UCHAR Usn;
    CLFS_CLIENT_ID ClientId;
    USHORT TotalSectorCount;
    USHORT ValidSectorCount;
    ULONG Padding;
    ULONG Checksum;
    ULONG Flags;
    CLFS_LSN CurrentLsn;
    CLFS_LSN NextLsn;
    ULONG RecordOffsets[16];
    ULONG SignaturesOffset;
} CLFS_LOG_BLOCK_HEADER, *PCLFS_LOG_BLOCK_HEADER;

typedef struct _CLFS_METADATA_RECORD_HEADER
{
    ULONGLONG ullDumpCount;
} CLFS_METADATA_RECORD_HEADER, *PCLFS_METADATA_RECORD_HEADER;

typedef enum _CLFS_EXTEND_STATE
{
    ClfsExtendStateNone,
    ClfsExtendStateExtendingFsd,
    ClfsExtendStateFlushingBlock
} CLFS_EXTEND_STATE, *PCLFS_EXTEND_STATE;

typedef struct _CLFS_TRUNCATE_CONTEXT
{
    CLFS_TRUNCATE_STATE eTruncateState;
    CLFS_CLIENT_ID cClients;
    CLFS_CLIENT_ID iClient;
    CLFS_LSN lsnOwnerPage;
    CLFS_LSN lsnLastOwnerPage;
    ULONG cInvalidSector;
} CLFS_TRUNCATE_CONTEXT, *PCLFS_TRUNCATE_CONTEXT;

typedef struct _CLFS_METADATA_BLOCK
{
    union
    {
        PUCHAR pbImage;
        ULONGLONG ullAlignment;
    };
    ULONG cbImage;
    ULONG cbOffset;
    CLFS_METADATA_BLOCK_TYPE eBlockType;
} CLFS_METADATA_BLOCK, *PCLFS_METADATA_BLOCK;

typedef struct _CLFS_CONTROL_RECORD
{
    CLFS_METADATA_RECORD_HEADER hdrControlRecord;
    ULONGLONG ullMagicValue;
    UCHAR Version;
    CLFS_EXTEND_STATE eExtendState;
    USHORT iExtendBlock;
    USHORT iFlushBlock;
    ULONG cNewBlockSectors;
    ULONG cExtendStartSectors;
    ULONG cExtendSectors;
    CLFS_TRUNCATE_CONTEXT cxTruncate;
    USHORT cBlocks;
    ULONG cReserved;
    CLFS_METADATA_BLOCK rgBlocks[ANYSIZE_ARRAY];
} CLFS_CONTROL_RECORD, *PCLFS_CONTROL_RECORD;

typedef struct _CLFS_BASE_RECORD_HEADER
{
    CLFS_METADATA_RECORD_HEADER hdrBaseRecord;
    CLFS_LOG_ID cidLog;
    ULONGLONG rgClientSymTbl[CLIENT_SYMTBL_SIZE];
    ULONGLONG rgContainerSymTbl[CONTAINER_SYMTBL_SIZE];
    ULONGLONG rgSecuritySymTbl[SHARED_SECURITY_SYMTBL_SIZE];
    ULONG cNextContainer;
    CLFS_CLIENT_ID cNextClient;
    ULONG cFreeContainers;
    ULONG cActiveContainers;
    ULONG cbFreeContainers;
    ULONG cbBusyContainers;
    ULONG rgClients[MAX_CLIENTS_DEFAULT];
    ULONG rgContainers[MAX_CONTAINERS_DEFAULT];
    ULONG cbSymbolZone;
    ULONG cbSector;
    USHORT bUnused;
    CLFS_LOG_STATE eLogState;
    UCHAR cUsn;
    UCHAR cClients;
} CLFS_BASE_RECORD_HEADER, *PCLFS_BASE_RECORD_HEADER;

typedef struct _CLFSHASHSYM
{
    CLFS_NODE_ID cidNode;
    ULONG ulHash;
    ULONG cbHash;
    ULONGLONG ulBelow;
    ULONGLONG ulAbove;
    LONG cbSymName;
    LONG cbOffset;
    BOOLEAN fDeleted;
} CLFSHASHSYM, *PCLFSHASHSYM;

typedef struct _CLFS_CLIENT_CONTEXT
{
    CLFS_NODE_ID cidNode;
    CLFS_CLIENT_ID cidClient;
    USHORT fAttributes;
    ULONG cbFlushThreshold;
    ULONG cShadowSectors;
    ULONGLONG cbUndoCommitment;
    LARGE_INTEGER llCreateTime;
    LARGE_INTEGER llAccessTime;
    LARGE_INTEGER llWriteTime;
    CLFS_LSN lsnOwnerPage;
    CLFS_LSN lsnArchiveTail;
    CLFS_LSN lsnBase;
    CLFS_LSN lsnLast;
    CLFS_LSN lsnRestart;
    CLFS_LSN lsnPhysicalBase;
    CLFS_LSN lsnUnused1;
    CLFS_LSN lsnUnused2;
    CLFS_LOG_STATE eState;
    union
    {
        HANDLE hSecurityContext;
        ULONGLONG ullAlignment;
    };
} CLFS_CLIENT_CONTEXT, *PCLFS_CLIENT_CONTEXT;


typedef struct _CLFS_CONTAINER_CONTEXT
{
    CLFS_NODE_ID cidNode;
    ULONGLONG cbContainer;
    CLFS_CONTAINER_ID cidContainer;
    CLFS_CONTAINER_ID cidQueue;
    union
    {
        CClfsContainer* pContainer;
        ULONGLONG ullAlignment;
    };
    CLFS_USN usnCurrent;
    CLFS_CONTAINER_STATE eState;
    ULONG cbPrevOffset;
    ULONG cbNextOffset;
} CLFS_CONTAINER_CONTEXT, *PCLFS_CONTAINER_CONTEXT;

typedef struct _CLFS_SHARED_SECURITY_CONTEXT
{
    CLFS_NODE_ID cidNode;
    ULONG cRef;
    ULONG cRefActive;
    ULONG coffDescriptor;
    ULONG cbDescriptor;
    UCHAR rgbSecurityDescriptor[ANYSIZE_ARRAY];
} CLFS_SHARED_SECURITY_CONTEXT, *PCLFS_SHARED_SECURITY_CONTEXT;

struct _CClfsBaseFile
{
    ULONG m_cRef;
    PUCHAR m_pbImage;
    ULONG m_cbImage;
    PERESOURCE m_presImage;
    USHORT m_cBlocks;
    PCLFS_METADATA_BLOCK m_rgBlocks;
    PUSHORT m_rgcBlockReferences;
    CLFSHASHTBL m_symtblClient;
    CLFSHASHTBL m_symtblContainer;
    CLFSHASHTBL m_symtblSecurity;
    ULONGLONG m_cbContainer;
    ULONG m_cbRawSectorSize;
    BOOLEAN m_fGeneralBlockReferenced;
} CClfsBaseFile, *PCLFSBASEFILE;

typedef struct _CLFSHASHTBL
{
    PULONGLONG rgSymHash;
    LONG cHashElt;
    CClfsBaseFile* pBaseFile;
} CLFSHASHTBL, *PCLFSHASHTBL;

typedef struct _CLFS_TRUNCATE_RECORD_HEADER
{
    CLFS_METADATA_RECORD_HEADER hdrBaseRecord;
    ULONG coffClientChange;
    ULONG coffOwnerPage;
} CLFS_TRUNCATE_RECORD_HEADER, *PCLFS_TRUNCATE_RECORD_HEADER;

typedef struct _CLFS_TRUNCATE_CLIENT_CHANGE
{
    CLFS_CLIENT_ID cidClient;
    CLFS_LSN lsn;
    CLFS_LSN lsnClient;
    CLFS_LSN lsnRestart;
    USHORT cLength;
    USHORT cOldLength;
    ULONG cSectors;
    CLFS_SECTOR_CHANGE rgSectors[ANYSIZE_ARRAY];
} CLFS_TRUNCATE_CLIENT_CHANGE, *PCLFS_TRUNCATE_CLIENT_CHANGE;

typedef struct _CLFS_SECTOR_CHANGE
{
    ULONG iSector;
    ULONG ulUnused;
    BYTE rgbSector[CLFS_SECTOR_SIZE];
} CLFS_SECTOR_CHANGE, *PCLFS_SECTOR_CHANGE;
