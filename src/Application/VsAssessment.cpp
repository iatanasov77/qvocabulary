#include "VsAssessment.h"

int VsAssessment::evaluate( int questionsNumber, int rightAnswersNumber )
{
	int assessment;
	int percents	= rightAnswersNumber * 100 / questionsNumber ;

	if ( percents >= ( 50 + ( 3 * ( 50 / 4 ) ) ) ) {
		assessment = 6;
	} else if ( percents >= ( 50 + ( 2 * ( 50 / 4 ) ) ) ) {
		assessment = 5;
	} else if ( percents >= ( 50 + ( 1 * ( 50 / 4 ) ) ) ) {
		assessment = 4;
	} else if ( percents >= 50 ) {
		assessment = 3;
	} else {
		assessment = 2;
	}
	
	return assessment;
}
