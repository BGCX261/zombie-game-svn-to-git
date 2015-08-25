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
		Job(void){}

		virtual ~Job(void){}

		virtual Job * run(Process * ){ return 0; }

		/// Get the id of the job factory for this class.
		virtual char getJobFactoryId(void) = 0;
	};

	/// 
	/** The JobFactory is an abstract class.  A derived type should be created
	*** for each separate derived job class.  The id must be unique and match
		* the id returned by the derived Job::getJobFactoryId().
	*/
	class JobFactory
	{
	public:
		virtual ~JobFactory(void){}

		/// Return the id of this JobFactory.
		virtual char id(void) const = 0;

		/// Create a job from the string.
		virtual Job * create(std::string const & data_) = 0;

		/// Serialise a Job into a string.
		virtual std::string create(Job * job_) = 0;
	};
}

#endif // _AM_Job_hpp_