//-----------------------------------------------------------------------------
// How does this work?
// Remember that before the compiler starts to process a source file, it runs
// a neat tool called a preprocessor on it. What this preprocessor does in
// this case is replace all the instances of *alloc/free with the expanded
// macros - this way we cleverly replace all the calls to the standard C
// memory (de)allocation functions. The same is done for new/delete
//
// Of course, we have the drawback that we can't name a member function of
// a class *alloc or free and we can't overload the new/delete operators without
// first undefining these macros - ah, a C++ preprocessor with RE replacement,
// that would be a dream come true :)
//
#ifndef _AutoMatic_MemMacros_h_
#define _AutoMatic_MemMacros_h_

#if _AutoMatic_Debug_Memory_Manager && AutoMatic_Debug_Mode
#   define new    (::AutoMatic::MemoryManager::instance().setOwner(__FILE__,__LINE__,__FUNCTION__),false) ? NULL                                                 : new
#   define delete (::AutoMatic::MemoryManager::instance().setOwner(__FILE__,__LINE__,__FUNCTION__),false) ? ::AutoMatic::MemoryManager::instance().setOwner("",0,"") : delete
#   define malloc(sz)      ::AutoMatic::MemoryManager::instance().allocMem(__FILE__,__LINE__,__FUNCTION__, ::AutoMatic::m_alloc_malloc, sz,      gProcessID)
#   define calloc(sz)      ::AutoMatic::MemoryManager::instance().allocMem(__FILE__,__LINE__,__FUNCTION__, ::AutoMatic::m_alloc_calloc, sz,      gProcessID)
#   define realloc(ptr,sz) ::AutoMatic::MemoryManager::instance().rllocMem(__FILE__,__LINE__,__FUNCTION__, ::AutoMatic::m_alloc_realloc,sz, ptr, gProcessID)
#   define free(ptr)       ::AutoMatic::MemoryManager::instance().dllocMem(__FILE__,__LINE__,__FUNCTION__, ::AutoMatic::m_alloc_free,       ptr, gProcessID)
#endif // _AutoMatic_Debug_Memory_Manager

#endif // _AutoMatic_MemMacros_h_
//-----------------------------------------------------------------------------