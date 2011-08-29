/*
  AeroQuad v3.0 - May 2011
  www.AeroQuad.com
  Copyright (c) 2011 Ted Carancho.  All rights reserved.
  An Open Source Arduino based multicopter.
 
  This program is free software: you can redistribute it and/or modify 
  it under the terms of the GNU General Public License as published by 
  the Free Software Foundation, either version 3 of the License, or 
  (at your option) any later version. 

  This program is distributed in the hope that it will be useful, 
  but WITHOUT ANY WARRANTY; without even the implied warranty of 
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
  GNU General Public License for more details. 

  You should have received a copy of the GNU General Public License 
  along with this program. If not, see <http://www.gnu.org/licenses/>. 
*/

#ifndef _AEROQUAD_ACCELEROMETER_APM_H_
#define _AEROQUAD_ACCELEROMETER_APM_H_

#include <Accelerometer.h>

void initializeAccel() {
  accelScaleFactor = G_2_MPS2((3.3/4096) / 0.330);
  accelSmoothFactor = 1.0;
}
  
void measureAccel() {
  int accelADC;
  for (byte axis = ROLL; axis < LASTAXIS; axis++) {
    const float rawADC = readADC(axis+3);
    if (rawADC > 500) { // Check if measurement good
      if (axis == ROLL)
        accelADC = rawADC - accelZero[axis];
      else
        accelADC = accelZero[axis] - rawADC;
      meterPerSec[axis] = filterSmooth(accelADC * accelScaleFactor, meterPerSec[axis], accelSmoothFactor);
    }
  }
}

void calibrateAccel() {
  int findZero[FINDZERO];
   
  for(byte calAxis = XAXIS; calAxis < LASTAXIS; calAxis++) {
    for (int i=0; i<FINDZERO; i++) {
      findZero[i] = readADC(calAxis+3);
      delay(2);
    }
    accelZero[calAxis] = findMedianInt(findZero, FINDZERO);
  }

  // replace with estimated Z axis 0g value
  accelZero[ZAXIS] = (accelZero[ROLL] + accelZero[PITCH]) / 2;
   
  // store accel value that represents 1g
  measureAccel();
  accelOneG = -meterPerSec[ZAXIS];
}

#endif
