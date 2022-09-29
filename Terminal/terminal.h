/*
 * Module Name: terminal.h
 * Description: header file of the terminal module
 *  Created on: Sep 15, 2022
 *      Author: Mahmoud Yasser
 */

#ifndef TERMINAL_H_
#define TERMINAL_H_
/*******************************************************************************
 *                                  Includes                                   *
 *******************************************************************************/
#include <stdint.h>
#include "../Card/card.h"
/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define 				TRANSACTION_DATE_SIZE			(10)
/*******************************************************************************
 *                               Configurations                                *
 *******************************************************************************/
/* a macro to define the maximum transaction amount */
#define 				MAX_AMOUNT						(10000.0f)
/*******************************************************************************
 *                              Defined Types                                  *
 *******************************************************************************/
typedef struct ST_terminalData_t {
	float transAmount;
	float maxTransAmount;
	uint8_t transactionDate[TRANSACTION_DATE_SIZE + 1];
} ST_terminalData_t;

typedef enum EN_terminalError_t {
	TERMINAL_OK,
	WRONG_DATE,
	EXPIRED_CARD,
	INVALID_CARD,
	INVALID_AMOUNT,
	EXCEED_MAX_AMOUNT,
	INVALID_MAX_AMOUNT
} EN_terminalError_t;

/*******************************************************************************
 *                      	Functions Prototypes                               *
 *******************************************************************************/
/*
 * Function Name: getTransactionDate
 * Description: This function gets the transaction date (today's date)
 * Transaction date is 10 characters string in the format DD/MM/YYYY, e.g 25/06/2022.
 * If the transaction date is NULL, less than 10 characters
 * or wrong format will return WRONG_DATE error, else return OK.
 */
EN_terminalError_t getTransactionDate(ST_terminalData_t *termData);

/*
 * Function Name: isCardExpired
 * Description: This function compares the card expiry date with the transaction date.
 * If the card expiration date is before the transaction date will return EXPIRED_CARD,
 * else return TERMINAL_OK.
 */
EN_terminalError_t isCardExpired(ST_cardData_t cardData,
		ST_terminalData_t termData);
/*
 * Function Name: isValidCardPAN
 * Description: This function checks whether the PAN is a Luhn number or not.
 * If PAN is not a Luhn number will return INVALID_CARD,
 * else return TERMINAL_OK.
 */
EN_terminalError_t isValidCardPAN(ST_cardData_t *cardData);

/*
 * Function Name: getTransactionAmount
 * Description: This function asks for the transaction amount
 * and saves it into terminal data.
 * If the transaction amount is less than or equal to 0 will return INVALID_AMOUNT,
 * else return TERMINAL_OK.
 */
EN_terminalError_t getTransactionAmount(ST_terminalData_t *termData);

/*
 * Function Name: isBelowMaxAmount
 * Description: This function compares the transaction amount with the terminal max amount.
 * If the transaction amount is larger than the terminal max amount will return EXCEED_MAX_AMOUNT,
 * else return TERMINAL_OK.
 */
EN_terminalError_t isBelowMaxAmount(ST_terminalData_t *termData);

/*
 * Function Name: setMaxAmount
 * Description: This function sets the maximum allowed amount into terminal data.
 * Transaction max amount is a float number.
 * If transaction max amount less than or equal to 0 will return INVALID_MAX_AMOUNT error,
 * else return TERMINAL_OK.
 */
EN_terminalError_t setMaxAmount(ST_terminalData_t *termData);

#endif /* TERMINAL_H_ */
