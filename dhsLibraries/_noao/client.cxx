/******************************************************************************
 * QUANTA - A toolkit for High Performance Data Sharing
 * Copyright (C) 2003 Electronic Visualization Laboratory,  
 * University of Illinois at Chicago
 *
 * This library is free software; you can redistribute it and/or modify it 
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either Version 2.1 of the License, or 
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public 
 * License for more details.
 * 
 * You should have received a copy of the GNU Lesser Public License along
 * with this library; if not, write to the Free Software Foundation, Inc., 
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 * Direct questions, comments etc about Quanta to cavern@evl.uic.edu
 *****************************************************************************/

#include <QUANTA/QUANTA.hxx>

#ifdef WIN32
#include <sys\timeb.h>
#elif __APPLE__
#include <sys/time.h>
#elif __FreeBSD__
#include <sys/types.h>
#include <sys/timeb.h>
#else
#include <sys/timeb.h>
#endif


int main(int argc, char** argv)
{
/*	QUANTAinit(); */

	QUANTAnet_remoteParallelFileIOClient32_c client;
	
	int day, mon, year, hour, min, sec;
	int day1, mon1, year1, hour1, min1, sec1;
    if (argc < 4)
    {
        printf("\nInsufficient number of arguments - usage: client <hostname> <localfilename> <remotefilename>\n");
        exit(1);
    }
    
	client.upload(argv[1], argv[2], argv[3], 10);

    return 1;
}
