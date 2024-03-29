/*
 * vi:ts=4:tw=78:shiftwidth=4:expandtab
 * vim600:fdm=marker
 *
 * display.cpp  -  a handy printf like routine
 *
 * Copyright (C) 2003 by Zhang Le <ejoy@users.sourceforge.net>
 * Begin       : 01-Jun-2003
 * Last Change : 24-Dec-2004.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

//#ifdef HAVE_CONFIG_H
//#include "config.h"
//#endif
#include "stdafx.h"

#include <cassert>
#include <iostream>
#include <cstdarg>
#include <stdio.h>
#include "display.hpp"

#include "..\\Include\\SMCom.h"
#include "..\\..\\SMatrix.h"
namespace maxent {

int verbose = 1;
CSMatrixApp *app = (CSMatrixApp *)AfxGetApp();
char charresponse[1024];

using namespace std;

// without newline
void displayA(const char *msg, ... )
{
    if (verbose) {
        char buf[1024];
        buf[1023] = '\0';
        va_list ap;
        va_start(ap, msg);			// use variable arg list
        _vsnprintf_s(buf, 1023, msg, ap);
        va_end(ap);
        //std::cout << buf << std::flush;
		//AppCall::Maxen_Responce_Message(buf);
		strcpy_s(charresponse, 1024, buf);
		::SendMessage((*(app->pdlg)).m_hWnd, WM_SRESPONCE_MESSAGE,(WPARAM)charresponse, 1);
    }
}

// with newline
void display(const char *msg, ... )
{
    if (verbose) {
        char buf[1024];
        buf[1023] = '\0';
        va_list ap;
        va_start(ap, msg);			// use variable arg list
        _vsnprintf_s(buf, 1023, msg, ap);
        va_end(ap);
        //std::cout << buf << std::endl;
		if(strlen(buf) < 1024){
			strcat_s(buf,1024, "\n");
		}
		//AppCall::Maxen_Responce_Message(buf);
		strcpy_s(charresponse, 1024, buf);
		::SendMessage((*(app->pdlg)).m_hWnd, WM_SRESPONCE_MESSAGE,(WPARAM)charresponse, 1);
    }
}
string display_align(const char *msg, int aligncnt, ... )
{
	
	int cnt = strlen(msg);
	if(!(aligncnt > cnt)){
		return "";
	}
	cnt = aligncnt-cnt;
	string outstr;
	for(int i = 0; i < cnt; i++){
		outstr += " ";
	}
	return outstr;
}

void display_with_Save(const char *msg, ... )
{
    if (verbose) {
        char buf[1024];
        buf[1023] = '\0';
        va_list ap;
        va_start(ap, msg);			// use variable arg list
        _vsnprintf_s(buf, 1023, msg, ap);
        va_end(ap);
        //std::cout << buf << std::endl;
		//AppCall::Maxen_Responce_Message_with_Save(buf);
		strcpy_s(charresponse, 1024, buf);
		::SendMessage((*(app->pdlg)).m_hWnd, WM_SRESPONCE_MESSAGE,(WPARAM)charresponse, 1);
    }
}

} // namespace maxent
