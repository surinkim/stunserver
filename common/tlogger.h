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

// Add implementation logger with boost::log
// hyunuk kim (nnhope@hotmail.com)

#ifndef TLOGGING_H
#define TLOGGING_H



const uint32_t TL_ERROR = 0;
const uint32_t TL_INFO  = 1;
const uint32_t TL_DEBUG = 2;
const uint32_t TL_TRACE = 3;

namespace TLogging
{
    uint32_t GetLogLevel();
    void SetLogLevel(uint32_t level);
    void LogMsg(uint32_t level, const char* pszFormat, ...);
}

#endif
