#ifndef _AutoMatic_Memory_h_
#define _AutoMatic_Memory_h_

#ifdef AUTOMATIC_MEMORY

#define AUTOMATIC_PLATFORM
#include "AM_Platform.h"


#define _AutoMatic_Debug_Memory_Manager 1

#if _AutoMatic_Debug_Memory_Manager && AutoMatic_Debug_Mode

//-----------------------------------------------------------------------------
// We have to declare the global new([])/delete([]) operators before declaring 
// the AutoMatic::MemoryManager class since it lists them as friend functions
void *operator new(size_t reportedSize);
void *operator new[](size_t reportedSize);
void operator delete(void *reportedAddress);
void operator delete[](void *reportedAddress);
//-----------------------------------------------------------------------------

namespace AutoMatic 
{
	#ifndef __FUNCTION__
		#define __FUNCTION__ "???"
	#endif

	/** For internal use only.
	\internal.
	@remarks
	This structure holds the allocation tracking information. So,
	for each allocation made, the overhead this memory manager adds
	is the size of this structure, the lengths of the names of the
	file and function in which the allocation was made and the
	padding size (which can be adjusted).
	*/
	typedef struct tag_au
	{
		size_t actualSize;
		size_t reportedSize;

		void *actualAddress;
		void *reportedAddress;

		char sourceFile[40];
		char sourceFunc[40];

		unsigned int sourceLine;
		unsigned int allocationType;

		bool breakOnDealloc;
		bool breakOnRealloc;

		unsigned int allocationNumber;
		unsigned int processID;

		struct tag_au *next;
		struct tag_au *prev;
	} sAllocUnit;

	typedef struct
	{
		size_t totalReportedMemory;
		size_t totalActualMemory;

		size_t peakReportedMemory;
		size_t peakActualMemory;

		size_t accumulatedReportedMemory;
		size_t accumulatedActualMemory;
		size_t accumulatedAllocUnitCount;

		size_t totalAllocUnitCount;
		size_t peakAllocUnitCount;
	} sMStats;

	enum
	{
		m_alloc_unknown        = 0,
		m_alloc_new            = 1,
		m_alloc_new_array      = 2,
		m_alloc_malloc         = 3,
		m_alloc_calloc         = 4,
		m_alloc_realloc        = 5,
		m_alloc_delete         = 6,
		m_alloc_delete_array   = 7,
		m_alloc_free           = 8
	};

	/** See the \ref memory_manager.
	*/
	class MemoryManager
	{
		friend void * ::operator new(size_t);
		friend void * ::operator new[](size_t);
		friend void ::operator delete(void*);
		friend void ::operator delete[](void*);

	public:
		static MemoryManager& instance(void);

	private:
		/// This is used in the process tracking part of the memory manager.
		unsigned m_uProcessIDs;
		/// This is set to true when deinitialization takes place.
		bool m_bDeinitTime;

#ifndef __BORLANDC__
	private:
#else
	public:
#endif
		//-------------------------------------------------------------------------
		// Wrappers for the new/delete functions        
		void *op_new_sc( size_t reportedSize, unsigned processID );
		void *op_new_vc( size_t reportedSize, unsigned processID );

		void *op_new_sc( size_t reportedSize, const char *sourceFile, int sourceLine, unsigned processID );
		void *op_new_vc( size_t reportedSize, const char *sourceFile, int sourceLine, unsigned processID );

		void op_del_sc( void *reportedAddress, unsigned processID );
		void op_del_vc( void *reportedAddress, unsigned processID );
		//-------------------------------------------------------------------------

		/** This function is intended for internal use only.
		\internal
		@remarks
		This function is used to return an unique handle for each process 
		calling it. The returned unsigned int is then passed to the memory
		manager every time a re/de/allocation request is made, in order
		to check that deallocations don't occur in processes other than the 
		ones in which allcations were made and so on.
		@par
		Actually, the problem of re/de/allocating in other processes was
		solved with the addition of the new memory manager, but you may
		want to limit the occurence of such events anyway, and this function
		helps you do just that.
		*/
		unsigned _getProcessID();

	public:
		MemoryManager();
		~MemoryManager();

		//-------------------------------------------------------------------------
		// Used by the macros     
		void setOwner(const char *file, const unsigned int line, const char *func);
		//-------------------------------------------------------------------------

		//-------------------------------------------------------------------------
		// Allocation breakpoints        
		bool &breakOnRealloc(void *reportedAddress);
		bool &breakOnDealloc( void *reportedAddress );
		void breakOnAlloc( unsigned int count );
		//-------------------------------------------------------------------------

		//-------------------------------------------------------------------------
		// The meat & potatoes of the memory tracking software

		/** This function is intended for internal use only.
		\internal
		@remarks
		This function is the actual memory allocator and acts as a bridge
		between AutoMatic and the C/C++ alloc/calloc functions. 
		@par
		While memory allocation requests are made trough this function, 
		the tracking of memory addresses is possible. Therefore, attempting 
		to deallocate a portion of memory that was not allocated using 
		this function will result in a warning given by the deallocator, 
		dllocMem.
		*/
		void * allocMem(
			const char *sourceFile, 
			const unsigned int sourceLine, 
			const char *sourceFunc,
			const unsigned int allocationType, 
			const size_t reportedSize, 
			const unsigned processID );

		/** This function is intended for internal use only.
		\internal
		@remarks
		This function is the actual memory reallocator and acts as a bridge
		between AutoMatic and the C/C++ realloc function. 
		@par
		While memory reallocation requests are made trough this function, 
		the tracking of memory addresses is possible. Therefore, attempting 
		to deallocate a portion of memory that was not reallocated using 
		this function will result in a warning given by the deallocator, 
		dllocMem. 
		@par
		As well, trying to reallocate memory that was not allocated using
		mallc/calloc will result in a warning.
		*/
		void * rllocMem(
			const char *sourceFile, 
			const unsigned int sourceLine, 
			const char *sourceFunc,
			const unsigned int reallocationType, 
			const size_t reportedSize, 
			void *reportedAddress, 
			const unsigned processID );

		/** This function is intended for internal use only.
		\internal
		@remarks
		This function is the actual memory deallocator and acts as a
		bridge between AutoMatic and the C/C++ free function.
		@par
		While memory deallocation requests are made trough this function, 
		the tracking of memory addresses is possible. Therefore, attempting 
		to deallocate a portion of memory that was not allocated using 
		allocMem or rllocMem, trying to deallocate memory that was
		allocated with malloc using delete (and the corresponding 
		permutations) or trying to deallocate memory allocated from from
		process will result in a warning.
		@note
		Actually, memory can be allocated in one process and deallocated
		in another, since the actual (de)allocation takes place in the
		memory space of the AutoMatic library. 
		@par
		Tracking this kind of (possible) errors exists because users may 
		want to write their own memory allocator later on or they'd like 
		to get rid of AutoMatic's memory allocator.
		*/
		void dllocMem(
			const char *sourceFile, 
			const unsigned int sourceLine, 
			const char *sourceFunc,
			const unsigned int deallocationType, 
			const void *reportedAddress, 
			const unsigned processID );
		//-------------------------------------------------------------------------

		//-------------------------------------------------------------------------
		// Utilitarian functions        
		bool validateAddr(const void *reportedAddress);
		bool validateAlloc(const sAllocUnit *allocUnit);
		bool validateAllAllocs();
		//-------------------------------------------------------------------------

		//-------------------------------------------------------------------------
		// Unused RAM calculations        
		unsigned int calcUnused( const sAllocUnit *allocUnit );
		unsigned int calcAllUnused();
		//-------------------------------------------------------------------------

		//-------------------------------------------------------------------------
		// Logging and reporting        
		void dumpAllocUnit( const sAllocUnit *allocUnit, const char *prefix = "" );
		void dumpMemReport( const char *filename = "memreport.log", const bool overwrite = true );
		sMStats getMemStats();            
		//-------------------------------------------------------------------------        
	};
}

/** This variable exists separately in each module that links to the AutoMatic library
and is used to track the ID of the current process from the perspective
of the memory manager.
@see
unsigned AutoMatic::MemoryManager::_getProcessID()
*/
static unsigned gProcessID = 0;

// When compiling in Visual C++ (occuring in VS2005 Express but not for VC 7.1) with
// managed C++, should put the new([])/delete([]) overrides inside unmanaged context,
// otherwise Visual C++ will link with overridden version of new([]) and CRT version
// of delete([]), thus, mess up both of AutoMatic memory manager and CRT memory manager.
#if defined(__cplusplus_cli)
#pragma managed(push, off)
#endif
//-----------------------------------------------------------------------------
// Overridden global new([])/delete([]) functions
//
inline void *operator new(size_t reportedSize)
{
	if( !gProcessID )
		gProcessID = AutoMatic::MemoryManager::instance()._getProcessID();
	return AutoMatic::MemoryManager::instance().op_new_sc( reportedSize, gProcessID );
}
inline void *operator new[](size_t reportedSize)
{
	if( !gProcessID )
		gProcessID = AutoMatic::MemoryManager::instance()._getProcessID();
	return AutoMatic::MemoryManager::instance().op_new_vc( reportedSize, gProcessID );
}

inline void operator delete(void *reportedAddress)
{
	AutoMatic::MemoryManager::instance().op_del_sc( reportedAddress, gProcessID );    
}
inline void operator delete[](void *reportedAddress)
{
	AutoMatic::MemoryManager::instance().op_del_vc( reportedAddress, gProcessID );
}
//-----------------------------------------------------------------------------
#if defined(__cplusplus_cli)
#pragma managed(pop)
#endif

//-----------------------------------------------------------------------------
// This header adds the *alloc/free macros, wrapping the C functions
#include "AM_MemMacros.h"
//-----------------------------------------------------------------------------

#else // _AutoMatic_Debug_Memory_Manager && AutoMatic_Debug_Mode
namespace AutoMatic 
{
	/** See the \ref memory_manager.
	*/
	class MemoryManager
	{
	public:
		static MemoryManager& instance(void);

		MemoryManager();
		~MemoryManager();

		/** Memory allocator - uses plain old malloc.
		*/
		void *allocMem( const char *szFile, size_t uLine, size_t count ) throw ( );

		/** Memory re-allocator - uses plain old realloc.
		*/
		void *rllocMem( const char *szFile, size_t uLine, void *ptr , size_t count ) throw ( );

		/** Memory allocator - uses plain old calloc.
		*/
		void *cllocMem( const char *szFile, size_t uLine, size_t num, size_t size ) throw ( );

		/** Memory de-allocator - uses plain old free.
		*/
		void dllocMem( const char *szFile, size_t uLine, void *ptr ) throw ( );
	};

}

#endif // _AutoMatic_Debug_Memory_Manager && AutoMatic_Debug_Mode

#else 

#ifdef _DEBUG
	#include <stdlib.h>
	#include <crtdbg.h>
#endif // _DEBUG

#endif // AUTOMATIC_MEMORY

#endif // _AutoMatic_Memory_h_
