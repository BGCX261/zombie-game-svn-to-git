//-----------------------------------------------------------------------------
// If you want to use the real C(++) *alloc/free/new/delete functions, you must
// include this header before the section that uses them.
#ifdef _AutoMatic_MemMacros_h_
#undef _AutoMatic_MemMacros_h_
#undef new
#undef delete
#undef malloc
#undef calloc
#undef realloc
#undef free
#endif
//-----------------------------------------------------------------------------
