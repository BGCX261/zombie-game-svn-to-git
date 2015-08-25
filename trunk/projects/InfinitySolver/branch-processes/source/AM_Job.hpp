#ifndef _AM_Job_hpp_
#define _AM_Job_hpp_

#include <string>

namespace AutoMatic
{
	class Process;

	/// 
	/** A Job is passed to a process, which runs it when it is able to.
	*** Job's require an associated JobFactory, to facilitate serialisation.
	*/
	class Job
	{
	public:
		Job(int const priority_) 
			: _priority(priority_), _parent(0)
		{}

		virtual ~Job(void){}

		virtual Job * run(Process * ){ return 0; }

		/// Get the id of the job factory for this class.
		virtual unsigned char getJobFactoryId(void) = 0;

		unsigned long const priority(void) const { return _priority; }
		
		Job * parent(void) const { return _parent; }
		void parent(Job * parent_){ _parent = parent_; }

	protected:
		unsigned long _priority;
		Job * _parent;
	};

	class JobPriority
	{
	public:
		bool operator()(Job * lhs_, Job * rhs_)
		{
			return lhs_->priority() > rhs_->priority();
		}
	};
}

#endif // _AM_Job_hpp_