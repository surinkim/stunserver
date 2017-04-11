/*
   Copyright 2011 John Selbie

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#define BOOST_LOG_DYN_LINK 1

#include "commonincludes.hpp"

#include "tlogger.h"

#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>

namespace logging = boost::log;
namespace src = boost::log::sources;
namespace expr = boost::log::expressions;
namespace keywords = boost::log::keywords;
namespace trivial = logging::trivial;

namespace TLogging
{
    //static uint32_t s_loglevel = LL_ALWAYS; // error and usage messages only

    void VPrintMsg(const char* pszFormat, va_list& args)
    {
        ::vprintf(pszFormat, args);
        ::printf("\n");
    }

    uint32_t GetLogLevel()
    {
        //return s_loglevel;
        return 0;
    }

    void SetLogLevel(uint32_t level)
    {
        logging::add_file_log (
            keywords::file_name = "sample_%N.log",
            keywords::rotation_size = 10 * 1024 * 1024, //10mb마다 rotate
            //keywords::time_based_rotation = logging::sinks::file::rotation_at_time_point(0, 0, 0), //12시마다 rotate
            //keywords::format = "[%TimeStamp%]: %Message%"
            keywords::format =
            (
                expr::stream
                    << expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d %H:%M:%S")
                    << ": <" << trivial::severity
                    << "> " << expr::smessage
            )

        );

        logging::add_console_log(
            std::cout,
            keywords::format =
            (
                expr::stream
                    << expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d %H:%M:%S")
                    << ": <" << trivial::severity
                    << "> " << expr::smessage
            )
        );

        if (level == 0)
        {
            logging::core::get()->set_filter(trivial::severity >= trivial::error);
        }
        else if (level == 1)
        {
            logging::core::get()->set_filter(trivial::severity >= trivial::info);
        }
        else if (level == 2)
        {
            logging::core::get()->set_filter(trivial::severity >= trivial::debug);
        }
        else if (level == 3)
        {
            logging::core::get()->set_filter(trivial::severity >= trivial::trace);
        }
        else
        {
            logging::core::get()->set_filter(trivial::severity >= trivial::error);
        }

        logging::add_common_attributes();
        
        
       // s_loglevel = level;
    }


    void LogMsg(uint32_t level, const char* pszFormat, ...)
    {
        va_list args;
        va_start(args, pszFormat);

        // if (level <= s_loglevel)
        // {
        //     VPrintMsg(pszFormat, args);
        // }

        //::vprintf(pszFormat, args);
        //::printf("\n");
        
        char szBuf[1024] = {0, };
        vsprintf(szBuf, pszFormat, args);

        src::severity_logger< trivial::severity_level > lg;
        if ( level == 0)
        {
            BOOST_LOG_SEV(lg, trivial::error) << szBuf;
        }
        else if (level == 1)
        {
            BOOST_LOG_SEV(lg, trivial::info) << szBuf;
        }
        else if (level == 2)
        {
            BOOST_LOG_SEV(lg, trivial::debug) << szBuf;
        }
        else if (level == 3)
        {
            BOOST_LOG_SEV(lg, trivial::trace) << szBuf;
        }
        else
        {
            BOOST_LOG_SEV(lg, trivial::error) << szBuf;
        }

        va_end(args);
    }
}
