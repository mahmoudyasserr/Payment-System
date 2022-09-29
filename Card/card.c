/*
 * Module Name: card.c
 * Description: source file of the card module
 * card.c
 *  Created on: Sep 15, 2022
 *      Author: Mahmoud Yasser
 */

/*******************************************************************************
 *                                  Includes                                   *
 *******************************************************************************/
#include "card.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "../Eclipse_Config.h"

/*******************************************************************************
 *                              Static Prototypes                              *
 *******************************************************************************/

/*
 * Function Name: checkDateFormat
 * Description: a static function to check the expiry data format is MM/YY
 */
static uint8_t checkExpiryDateFormat(uint8_t *date);

/*
 * Function Name: isDigitsOnly
 * Description: a static function to check whether a PAN has
 * 				no characters other than numerical digits or not
 * 				it returns 0 if it has alphabets otherwise it returns 1
 */
static uint8_t isDigitsOnly(uint8_t *pan);

/*******************************************************************************
 *                                  Functions                                  *
 *******************************************************************************/
EN_cardError_t getCardHolderName(ST_cardData_t *cardData) {
	EN_cardError_t status = CARD_OK;
	uint8_t name_size = 0;
	uint8_t input[MAX_NAME_SIZE + 6];
	if (cardData != NULL) {
		printf("%s", "Enter card holder name (20 to 24 characters): \n");
#if (ECLIPSE_USED == 1)
		fflush(stdout);
#endif
		while (getchar() != '\n')
			continue;
		gets(input);
		name_size = strlen(input);
		/* checking name input format: */
		/* if any character other than a space is not a character, the input is invalid */
		for (uint8_t i = 0; ((i < name_size) && (status == CARD_OK)); i++) {
			if ((input[i] != ' ') && (isalpha(input[i]) == 0)) {
				status = WRONG_NAME;
			}
		}
		/* if input size > max or < min, the input is invalid */
		if ((cardData->cardHolderName == NULL) || (name_size < MIN_NAME_SIZE)
				|| (name_size > MAX_NAME_SIZE)) {
			status = WRONG_NAME;

		}
		/* if the input is valid, copy it cardData structure */
		else {
			strcpy(cardData->cardHolderName, input);
		}
	} else {
		status = WRONG_NAME;
	}
	return status;
}

static uint8_t checkExpiryDateFormat(uint8_t *date) {
	uint8_t return_val = 0;
	/*
	 * extracting the first and the second digit of the month
	 * subtracting 48 to convert from ASCII to normal integer
	 */
	uint8_t month_digit0 = date[0] - 48;
	uint8_t month_digit1 = date[1] - 48;
	/* getting the month number */
	uint8_t month = month_digit0 * 10 + month_digit1;
	/* checking if input format is MM/YY:
	 * if the first and the last two digits are not numbers, the input is invalid
	 * if the digit number 2 is not a '/', the input is invalid
	 * if the month is less than 0 or grater than 12, the input is invalid */
	if ((isdigit(date[0])) && (isdigit(date[1])) && (isdigit(date[3]))
			&& (isdigit(date[4])) && (date[2] == '/') && (month > 0)
			&& (month <= 12)) {
		return_val = 1;
	}
	return return_val;
}

EN_cardError_t getCardExpiryDate(ST_cardData_t *cardData) {
	EN_cardError_t status = CARD_OK;
	uint8_t input[EXP_DATE_SIZE + 5];
	uint8_t input_size = 0;

	if (cardData != NULL) {
		printf("%s", "Enter a 5-character expiry date in the format MM/YY: \n");
#if (ECLIPSE_USED == 1)
		fflush(stdout);
#endif
		scanf("%s", input);
		input_size = strlen(input);
		/* checking if the input is valid
		 * if the input size is not equal to EXP_DATE_SIZE (5), the input is invalid
		 * if the input format is invalid, the input is invalid */
		if ((cardData->cardExpirationDate == NULL)
				|| (input_size != EXP_DATE_SIZE)
				|| (checkExpiryDateFormat(input) == 0)) {
			status = WRONG_EXP_DATE;
		}
		/* if the input is valid, it is copied to cardDate structure */
		else {
			strcpy(cardData->cardExpirationDate, input);
		}
	} else {
		status = WRONG_EXP_DATE;
	}
	return status;
}

static uint8_t isDigitsOnly(uint8_t *pan) {
	uint8_t return_val = 1;
	uint8_t i = 0;
	while ((pan[i] != '\0') && return_val != 0) {
		if (!isdigit(pan[i])) {
			return_val = 0;
		}
		i++;
	}
	return return_val;
}

EN_cardError_t getCardPAN(ST_cardData_t *cardData) {
	EN_cardError_t status = CARD_OK;
	uint8_t input[MAX_PAN_SIZE + 5];
	uint8_t input_size = 0;

	if (cardData != NULL) {
		printf("%s", "Enter PAN (16 to 19 characters): \n");
#if (ECLIPSE_USED == 1)
		fflush(stdout);
#endif
		scanf("%s", input);
		input_size = strlen(input);
		/* checking if the PAN is valid
		 * if the input size is less that the min or greater than the max, the input is invalid
		 * if the input has any character which is not a number, the input is invalid*/
		if ((cardData->primaryAccountNumber == NULL)
				|| (input_size < MIN_PAN_SIZE) || (input_size > MAX_PAN_SIZE)
				|| isDigitsOnly(input) == 0) {
			status = WRONG_PAN;
		} else {
			/* if the input is valid, it is copied to cardDate structure */
			strcpy(cardData->primaryAccountNumber, input);
		}
	} else {
		status = WRONG_EXP_DATE;
	}
	return status;
}
