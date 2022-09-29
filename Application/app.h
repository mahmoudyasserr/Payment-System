/*
 * Module Name: app.h
 * Description: the header file of the application code
 *  Created on: Sep 15, 2022
 *      Author: Mahmoud Yasser
 */

#ifndef APP_H_
#define APP_H_

/*******************************************************************************
 *                              Defined Types                                  *
 *******************************************************************************/
typedef enum EN_appError_t {
	APP_OK,
	APP_ERROR
} EN_appError_t;
/*******************************************************************************
 *                      	Functions Prototypes                               *
 *******************************************************************************/
void appStart(void);

#endif /* APP_H_ */
