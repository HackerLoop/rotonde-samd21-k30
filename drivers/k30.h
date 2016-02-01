/*
 * k30.h
 *
 * Created: 27/01/2016 20:42:15
 *  Author: GroundLayer
 */ 


#ifndef K30_H_
#define K30_H_

#include "asf.h"

bool k30_init(void);

bool k30_read(float* CO2);

#endif /* K30_H_ */