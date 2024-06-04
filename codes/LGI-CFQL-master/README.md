LGI-CFQL-master
==========

This code implements LGI-CFQL. We have compiled the code to 'lgi' with g++ 7.5.0, and you can run it directly.

To run the algorithm, first type

"chmod +x lgi"

and then

./lgi 1 data/queries.out data/dataset.out data/indexes.out data/queries_result.out estimation0_mae estimation1_mae estimation2_mae 

where queries.out is the query test set, dataset.out is the data set, indexes.out is the constructed index, queries_result.out is the output file of query result.
The estimation0_mae is a value (it comes from "results\xx\qp-LG-Index\estimation model\cluster_0.PNG: Test MAE"), 
which is the estimated error of the estimated model GIN 0; estimation1_mae is comes from "results\xx\qp-LG-Index\estimation model\cluster_1.PNG: Test MAE"), 
which is the estimated error of the estimated model GIN 1; estimation2_mae is comes from "results\xx\qp-LG-Index\estimation model\cluster_2.PNG: Test MAE"), 
which is the estimated error of the estimated model GIN 2; 

As an example:

./lgi 1 data/V3.0_AIDS_heavy_test.out data/AIDS.graph data/lgi_87%_33646.out data/queries_result.out 583.3433 0.245 0.2331

In data/, there are some files:
cluster_0/1/2_para_end_assp.txt are the parameter files of the trained GIN 0/1/2 models. 
model_para_end_ffm.txt is the parameter file of the trained coupled model for fingerprinting and filtering.  
RFModelfile.xml is the parameter file of the trained random forest model for index selection.
statistic.out is the statistics of clusters in task dispatcher.
These files are all available in the 'results/' directory.