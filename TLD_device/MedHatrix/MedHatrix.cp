/*
 *  MedHatrix.cp
 *  MedHatrix
 *
 *  Created by Sandeep Kaur on 30/04/13.
 *
 *
 */

#include <iostream>
#include "MedHatrix.h"
#include "MedHatrixPriv.h"

void MedHatrix::HelloWorld(const char * s)
{
	 MedHatrixPriv *theObj = new MedHatrixPriv;
	 theObj->HelloWorldPriv(s);
	 delete theObj;
};

void MedHatrixPriv::HelloWorldPriv(const char * s) 
{
	std::cout << s << std::endl;
};

