/*
 * Module Name: terminal.c
 * Description: source file of the terminal module
 *  Created on: Sep 15, 2022
 *      Author: Mahmoud Yasser
 */
/*******************************************************************************
 *                                  INCLUDES                                   *
 *******************************************************************************/
#include "terminal.h"
#include <stdio.h>
#include <ctype.h>
#include <time.h>
#include <string.h>

#include "../Eclipse_Config.h"
/*******************************************************************************
 *                              Static Prototypes                              *
 *******************************************************************************/
/*
 * Function Name: checkTransactionDateFormat
 * Description: a static function to check whether a transaction date has
 * 				the right format (DD/MM/YYYY) or not
 * 				it returns 0 if it invalid otherwise it returns 1
 */
static uint8_t checkTransactionDateFormat(uint8_t *str);
/*
 * Function Name: sumDigits
 * Description: a static function to sum the digits of a number
 */
static uint16_t sumDigits(uint32_t number);
/*
 * Function Name: launChecker
 * Description: a static function check whether a number is Luhn or not.
 */
static uint8_t luhnChecker(uint8_t *pan);

/*******************************************************************************
 *                                  Functions                                  *
 *******************************************************************************/

static uint8_t checkTransactionDateFormat(uint8_t *date) {
	uint8_t return_val = 1;

	for (uint8_t i = 0; (i < TRANSACTION_DATE_SIZE) && (return_val == 1); i++) {
		/* if digit 2 or digit 5 are not '/', the input is invalid */
		if ((i == 2) || (i == 5)) {
			if (date[i] != '/') {
				return_val = 0;
			}
		}
		/* if other digits are not numerical, the input is invalid */
		else {
			if (!(isdigit(date[i]))) {
				return_val = 0;
			}
		}
	}
	return return_val;
}

EN_terminalError_t getTransactionDate(ST_terminalData_t *termData) {
	EN_terminalError_t status = TERMINAL_OK;
	uint8_t date[TRANSACTION_DATE_SIZE + 1];
	uint8_t date_size = 0;
	uint8_t current_day = 0;
	uint8_t current_month = 0;
	uint16_t current_year = 0;
	/* assigning the current date in a structure*/
	time_t t = time(NULL);
	struct tm current_date = *localtime(&t);

	if (termData != NULL) {
		current_day = current_date.tm_mday;
		/* adding one to the month because they start from 0 */
		current_month = current_date.tm_mon + 1;
		/* adding 1900 to the year because they start from 1900 */
		current_year = current_date.tm_year + 1900;
		/* putting day, month, and year in the format DD/MM/YYYY */
		sprintf(date, "%.2d/%.2d/%d", current_day, current_month, current_year);
		date_size = strlen(date);
		/* checking if the date is valid
		 * if the size is not equal to TRANSACTION_DATE_SIZE, the input is invalid
		 * if the date format is not DD/MM/YYYY, the input is invalid */
		if ((termData->transactionDate == NULL)
				|| (date_size != TRANSACTION_DATE_SIZE)
				|| (checkTransactionDateFormat(date) == 0)) {
			status = WRONG_DATE;
		} else {
			/* if the input is valid, it is copied to the termData structure */
			strcpy(termData->transactionDate, date);

		}
	} else {
		status = WRONG_DATE;
	}
	return status;
}

EN_terminalError_t isCardExpired(ST_cardData_t cardData,
		ST_terminalData_t termData) {
	EN_terminalError_t return_val = TERMINAL_OK;
	uint8_t expiration_month_digits[2] = { 0 };
	uint8_t expiration_year_digits[2] = { 0 };
	uint8_t transaction_month_digits[2] = { 0 };
	uint8_t transaction_year_digits[2] = { 0 };
	uint8_t expiration_month = 0;
	uint8_t expiration_year = 0;
	uint8_t transaction_month = 0;
	uint8_t transaction_year = 0;

	/*
	 * extracting the digits of expiration month, expiration year, transaction month,
	 * and transaction year
	 * subtracting 48 to convert from ASCII to normal integer
	 */
	expiration_month_digits[0] = cardData.cardExpirationDate[0] - 48;
	expiration_month_digits[1] = cardData.cardExpirationDate[1] - 48;
	expiration_year_digits[0] = cardData.cardExpirationDate[3] - 48;
	expiration_year_digits[1] = cardData.cardExpirationDate[4] - 48;
	transaction_month_digits[0] = termData.transactionDate[3] - 48;
	transaction_month_digits[1] = termData.transactionDate[4] - 48;
	transaction_year_digits[0] = termData.transactionDate[8] - 48;
	transaction_year_digits[1] = termData.transactionDate[9] - 48;

	/*
	 * constructing the expiration month, expiration year, transaction month,
	 * and transaction year numbers
	 */
	expiration_month = expiration_month_digits[0] * 10
			+ expiration_month_digits[1];
	expiration_year = expiration_year_digits[0] * 10
			+ expiration_year_digits[1];
	transaction_month = transaction_month_digits[0] * 10
			+ transaction_month_digits[1];
	transaction_year = transaction_year_digits[0] * 10
			+ transaction_year_digits[1];

	/*
	 * if the expiration year is less than the transaction year
	 * or the both are equal and the expiration month is less than
	 * the transaction month, the card is expired.
	 */
	if ((expiration_year < transaction_year)
			|| ((expiration_year == transaction_year)
					&& (expiration_month < transaction_month))) {
		return_val = EXPIRED_CARD;
	}

	return return_val;
}

static uint16_t sumDigits(uint32_t number) {
	uint16_t sum = 0;
	while (number != 0) {
		sum += number % 10;
		number /= 10;
	}
	return sum;
}

static uint8_t luhnChecker(uint8_t *pan) {
	uint8_t return_val = 0;
	uint8_t digit = 0;
	uint16_t sum = 0;
	/* summing (odd digits + sum of digits(of even digits multiplied by 2)) */
	for (uint8_t i = 0; pan[i] != '\0'; i++) {
		digit = pan[i] - 48;
		if (i % 2 == 0) {
			digit *= 2;
			if (digit > 9) {
				digit = sumDigits(digit);
			}
		}
		sum += digit;
	}
	/* if the sum is divisible by 10, the number is Luhn */
	if (sum % 10 == 0) {
		return_val = 1;
	}
	return return_val;
}

EN_terminalError_t isValidCardPAN(ST_cardData_t *cardData) {
	EN_terminalError_t status = TERMINAL_OK;
	/* if the PAN is not Luhn, the card is invalid */
	if (luhnChecker(cardData->primaryAccountNumber) == 0) {
		status = INVALID_CARD;
	}
	return status;
}

EN_terminalError_t getTransactionAmount(ST_terminalData_t *termData) {
	EN_terminalError_t status = TERMINAL_OK;
	float input = 0.0;

	if (termData != NULL) {
		printf("%s", "Enter the transaction amount: \n");
#if (ECLIPSE_USED == 1)
		fflush(stdout);
#endif
		scanf("%f", &input);
		/* if the input is less than or equal to 0, the input is invalid */
		if (input <= 0) {
			status = INVALID_AMOUNT;
		} else {
			/* if the input is valid, it is copied to the termData structure */
			termData->transAmount = input;
		}
	} else {
		status = INVALID_AMOUNT;
	}
	return status;
}

EN_terminalError_t isBelowMaxAmount(ST_terminalData_t *termData) {
	EN_terminalError_t status = TERMINAL_OK;

	if (termData != NULL) {
		/* checking if the transaction amount exceeds the maximum transaction amount
		 */
		if (termData->transAmount > termData->maxTransAmount) {
			status = EXCEED_MAX_AMOUNT;
		}
	} else {
		status = EXCEED_MAX_AMOUNT;
	}
	return status;
}

EN_terminalError_t setMaxAmount(ST_terminalData_t *termData) {
	EN_terminalError_t status = TERMINAL_OK;
	if (termData != NULL) {

		/* checking if the maximum transaction amount is less than or equal to 0
		 * if so, the maximum amount is invalid
		 */
		if (MAX_AMOUNT <= 0.0) {
			status = INVALID_MAX_AMOUNT;
		}
		/* if the maximum transaction amount is valid, it is assigned to the termData structure */
		else {

			termData->maxTransAmount = MAX_AMOUNT;
		}
	} else {
		status = INVALID_MAX_AMOUNT;
	}
	return status;
}

