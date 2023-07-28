#ifndef _USER_INTERACTION_RF_2_H_
#define _USER_INTERACTION_RF_2_H_

#include "RandomCLoquatForests.h"
#include "RandomRLoquatForests.h"


int ReadClassificationForestConfigFile2(const char *configXMLPath, RandomCForests_info &RF_info);
int ReadRegressionForestConfigFile2(const char *configXMLPath, RandomRForests_info &RF_info);


int InitalClassificationDataMatrixFormFile2(const char *fileName, float **&data, int *&label, Dataset_info_C &data_info);

int InitalRegressionDataMatrixFormFile2(const char* fileName, float**& data, float*& target, Dataset_info_R& data_info);



void SaveRandomClassificationForestModel(const char *pFilePath, LoquatCForest *loquatForest, int outputType=0);
void SaveRandomRegressionForestModel(const char *pFilePath, LoquatRForest *loquatForest, int outputType=0);

int BuildRandomClassificationForestModel(const char *pFilePath, int fileType, LoquatCForest *&loquatForest);
int BuildRandomRegressionForestModel(const char *pFilePath, int fileType, LoquatRForest *&loquatForest);

#endif /* _USER_INTERACTION_RF_2_H_ */