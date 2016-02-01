/*
 * k30.c
 *
 * Created: 27/01/2016 20:51:54
 *  Author: GroundLayer
 */ 


#include "modules/drivers/k30.h"
#include "modules/modules/k30.h"

#include "registrar/registrar.h"
#include "tasks/modules.h"

#define MODULE_NAME "k30"
#define MODULE_DATA_SIZE 1
#define MODULE_DATA "co2"

data_name k30_names[MODULE_DATA_SIZE] = { MODULE_DATA  };
data_type k30_types[] = { FLOAT};
	
	void k30_tick(void);
	definition *k30_def;
	
	void k30_module_init(void) {
		k30_init();
		k30_def = register_definition(EVENT , MODULE_NAME , k30_names , k30_types , MODULE_DATA_SIZE , NULL, k30_tick , 5000);
	}
	
	void k30_tick(void) {
		float CO2;
		
		if(k30_read(&CO2)) {
			instance inst = create_instance(MODULE_NAME);
			if(inst.def != NULL && inst.values != NULL){
				(*(float*) inst.values[0]) = CO2;
				portBASE_TYPE xStatus = xQueueSendToBack(module_out, &inst , 10 / portTICK_RATE_MS );
				if(xStatus != pdPASS){
					release_instance(inst);
				}
			}
		}
		vTaskDelay(100);
	}