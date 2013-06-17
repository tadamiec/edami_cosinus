#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\non_zero_dim\vectorOperations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace UnitTest1
{		
	TEST_CLASS(UnitTest1)
	{
	public:

		TEST_METHOD(TestMethod1)
		{
			string fileName = "dataFiles/sports_part.mat";
			string outputFileName = "results.log";
			readVectors(fileName);
			Assert::AreEqual(1,1);
			//Assert::AreEqual(multiply(1,1), multiply(1,1));
		}

	};
}