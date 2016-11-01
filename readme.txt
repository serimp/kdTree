1.Instruction for compilation:

(1) mkdir build
(2) cd build
(3) cmake ..
(4) make

This will create executables under:

(1) kdtree/build/build_kdtree/build_kdtree
(2) kdtree/build/query_kdtree/query_kdtree




2. Instruction for running build_kdtree

------------------------------------------------------
./build_kdtree sample_data.csv model.csv
------------------------------------------------------
build_kdtree gets two arguments:

(1) The absolute path to the train data (.csv)
(2) The absolute path to save the model (.csv)

Alternatively, the sample_data.csv in examples folder can be loaded by typing '1' when prompted, e.g.
------------------------------------------------------
(1) ./build_kdtree
(2) 1
------------------------------------------------------
Note that this only works when ./build_tree is called from build/build_tree directory (because the data path is specified in relative file path).




3. Instruction for running query_kdtree

------------------------------------------------------
./query_kdtree sample_data.csv model.csv query_data.csv query_result.csv
------------------------------------------------------

query_kdtree gets 4 arguments from the console:

(1) The absolute path to the train data (.csv)
(2) The absolute path to the kdtree model (.csv)
(3) The absolute path to the test data (.csv)
(4) The absolute path to the knn search result to be saved (.csv)

Alternatively, the query_data.csv and precomputed_model.csv in examples folder can be loaded by typing '1' when prompted, e.g.
------------------------------------------------------
(1) ./query_kdtree
(2) 1
------------------------------------------------------
Note that this only works when ./query_tree is called from build/query_tree directory. (because the data path is specified in relative file path).



4. Instruction for more examples

100 different instances of sample_data and query_data as well as their ground truth 1 nearest neighbor result exists at /examples/more_examples/.

