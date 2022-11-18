#ifndef TEST_H
#define TEST_H

#include <iostream>
#include <assert.h>
#include "GameConfParser.h"

// Run all the quick test preliminaries.
void athena_quick_tests()
{
	std::cout << "Running Unit Tests" << std::endl;

	// Run Assertions
	ConfParser::ConfParser_tests();
}

#endif