/*
 * 	Module Name: card.h
 *	Description: header file of card module
 *  Created on: Sep 15, 2022
 *      Author: Mahmoud Yasser
 */

#ifndef CARD_H_
#define CARD_H_

/*******************************************************************************
 *                                  INCLUDES                                   *
 *******************************************************************************/
#include <stdint.h>
/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define 		MAX_NAME_SIZE			(24)
#define 		MIN_NAME_SIZE			(20)
#define 		EXP_DATE_SIZE			(5)
#define 		MAX_PAN_SIZE			(19)
#define 		MIN_PAN_SIZE			(16)
/*******************************************************************************
 *                              Defined Types                                  *
 *******************************************************************************/
typedef struct ST_cardData_t {
	uint8_t cardHolderName[MAX_NAME_SIZE + 1];
	uint8_t primaryAccountNumber[MAX_PAN_SIZE + 1];
	uint8_t cardExpirationDate[EXP_DATE_SIZE + 1];
} ST_cardData_t;

typedef enum EN_cardError_t {
	CARD_OK,
	WRONG_NAME,
	WRONG_EXP_DATE,
	WRONG_PAN
} EN_cardError_t;

/*******************************************************************************
 *                      	Functions Prototypes                               *
 *******************************************************************************/
/*
 * Function Name: getCardHolderName
 * Description: This function will ask for the cardholder's name and store it into card data.
 * Card holder name is 24 characters string max and 20 min.
 * If the cardholder name is NULL, less than 20 characters
 * or more than 24 will return WRONG_NAME error,
 * else return CARD_OK.
 */
EN_cardError_t getCardHolderName(ST_cardData_t *cardData);

/*
 * Function Name: getCardExpiryDate
 * Description: This function will ask for the card expiry date and store it in card data.
 * Card expiry date is 5 characters string in the format "MM/YY", e.g "05/25".
 * If the card expiry date is NULL, less or more than 5 characters,
 * or has the wrong format will return WRONG_EXP_DATE error, else return CARD_OK.
 */
EN_cardError_t getCardExpiryDate(ST_cardData_t *cardData);

/*
 * Function Name: getCardPAN
 * This function will ask for the card's Primary Account Number and store it in card data.
 * PAN is 20 characters alphanumeric only string 19 character max, and 16 character min.
 * If the PAN is NULL, less than 16 or more than 19 characters,
 * will return WRONG_PAN error, else return CARD_OK.
 */
EN_cardError_t getCardPAN(ST_cardData_t *cardData);

#endif /* CARD_H_ */
