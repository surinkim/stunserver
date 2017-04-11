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
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>


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
        //BOOST_LOG_TRIVIAL(debug) << "A debug severity message";
        boost::log::add_file_log ( 
            boost::log::keywords::file_name = "sample_%N.log", 
            boost::log::keywords::rotation_size = 10 * 1024 * 1024, //10mb마다 rotate 
            //boost::log::keywords::time_based_rotation = boost::log::sinks::file::rotation_at_time_point(0, 0, 0), //12시마다 rotate 
            //boost::log::keywords::format = "[%TimeStamp%]: %Message%" 
            boost::log::keywords::format =
            (
                boost::log::expressions::stream
                    << boost::log::expressions::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d %H:%M:%S")
                    << ": <" << boost::log::trivial::severity
                    << "> " << boost::log::expressions::smessage
            )

        );

        if (level == 0)
        {
            boost::log::core::get()->set_filter( boost::log::trivial::severity >= boost::log::trivial::error);
        }
        else if (level == 1)
        {
            boost::log::core::get()->set_filter(boost::log::trivial::severity >= boost::log::trivial::info);
        }
        else if (level == 2)
        {
            boost::log::core::get()->set_filter(boost::log::trivial::severity >= boost::log::trivial::debug);
        }
        else if (level == 3)
        {
            boost::log::core::get()->set_filter(boost::log::trivial::severity >= boost::log::trivial::trace);
        }
        else
        {
            boost::log::core::get()->set_filter(boost::log::trivial::severity >= boost::log::trivial::error);
        }

        boost::log::add_common_attributes();
        
        
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

        boost::log::sources::severity_logger< boost::log::trivial::severity_level > lg;
        if ( level == 0)
            BOOST_LOG_SEV(lg, boost::log::trivial::error) << szBuf;
        else if (level == 1)
            BOOST_LOG_SEV(lg, boost::log::trivial::info) << szBuf;
        else if (level == 2)
            BOOST_LOG_SEV(lg, boost::log::trivial::debug) << szBuf;
        else if (level == 3)
            BOOST_LOG_SEV(lg, boost::log::trivial::trace) << szBuf;
        else
            BOOST_LOG_SEV(lg, boost::log::trivial::error) << szBuf;


        va_end(args);
    }
}
