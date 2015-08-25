#ifndef _AM_FileWatcher_h_
#define _AM_FileWatcher_h_

#include <string>
#include <vector>

#define _WIN32_WINNT 0x0501
#include <windows.h>

#include <time.h>

namespace AutoMatic
{

	class Reporter;
	class FileWatcher
	{
	public:
		typedef std::vector< std::string > StringVec;
		FileWatcher(const std::string& directoryToWatch, int const resultBufferSize_ = 4096);
		~FileWatcher();

		bool Update(bool block_ = false);

		void InstallChangedFileSetCallback(Reporter * cb_) { m_cb = cb_; }

	private:
		bool Listen();
		void UpdateReportTiming();
		void ReportChangedFile(const std::string& filename);

		HANDLE m_directoryHandle;
		HANDLE m_notifyEvent;
		OVERLAPPED m_overlappedIo;
		char *m_notifyResults;

		StringVec m_pendingChangedFiles;
		__time64_t m_timeOfFirstPendingChange;
		bool m_timing;

		Reporter * m_cb;

		int const _resultBufferSize;

	private:
		FileWatcher(const FileWatcher&);
		FileWatcher& operator =(const FileWatcher&);
	};

	class Reporter
	{
	public:
		virtual ~Reporter(void){}

		virtual void operator()(FileWatcher::StringVec const & changedFiles_) = 0;
	};

}

#endif // _AM_FileWatcher_h_
