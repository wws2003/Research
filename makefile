MAKE_DIR = $(PWD)

SHARED_THIRD_PARTIES_DIR := $(MAKE_DIR)/SharedThirdParties
MATH_CONCEPT_DIR := $(MAKE_DIR)/ModuleMathConcepts/Debug
MATRIX_OPERATOR_DIR := $(MAKE_DIR)/ModuleOperator/Debug
IO_DIR := $(MAKE_DIR)/ModuleIO/Debug
ALGORITHM_DIR := $(MAKE_DIR)/ModuleAlgorithm/Debug
PARALLELISM_DIR := $(MAKE_DIR)/ModuleParallelism/Debug
EVALUATE_DIR := $(MAKE_DIR)/ModuleEvaluate/Debug
TEST_MATRIX_OPERATOR_DIR := $(MAKE_DIR)/TestMatrixOperator/Debug
TEST_EVALUATE_DIR := $(MAKE_DIR)/TestEvaluator/Debug
APPLICATION_DIR := $(MAKE_DIR)/GateApproximationApp/Debug

MAKE_FILE_PATH = makefile

all: testMatrixOperator testEvaluate app

testMatrixOperator: mathConcept matrixOperator
	@$(MAKE) -C $(TEST_MATRIX_OPERATOR_DIR) -f $(MAKE_FILE_PATH) all	

mathConcept: sharedThirdParties 
	@$(MAKE) -C $(MATH_CONCEPT_DIR) -f $(MAKE_FILE_PATH) all	

matrixOperator: sharedThirdParties
	@$(MAKE) -C $(MATRIX_OPERATOR_DIR) -f $(MAKE_FILE_PATH) all	

testEvaluate: io algorithm parallelism evaluate
	@$(MAKE) -C $(TEST_EVALUATE_DIR) -f $(MAKE_FILE_PATH) all	

io: sharedThirdParties
	@$(MAKE) -C $(IO_DIR) -f $(MAKE_FILE_PATH) all

evaluate: sharedThirdParties
	@$(MAKE) -C $(EVALUATE_DIR) -f $(MAKE_FILE_PATH) all	

algorithm: sharedThirdParties
	@$(MAKE) -C $(ALGORITHM_DIR) -f $(MAKE_FILE_PATH) all	

parallelism: 
	@$(MAKE) -C $(PARALLELISM_DIR) -f $(MAKE_FILE_PATH) all	

app:
	@$(MAKE) -C $(APPLICATION_DIR) -f $(MAKE_FILE_PATH) all	

sharedThirdParties:
	@$(MAKE) -C $(SHARED_THIRD_PARTIES_DIR) all

.PHONY: clean

clean:
	@$(MAKE) -C $(APPLICATION_DIR) -f $(MAKE_FILE_PATH) clean
	@$(MAKE) -C $(TEST_MATRIX_OPERATOR_DIR) -f $(MAKE_FILE_PATH) clean	
	@$(MAKE) -C $(TEST_EVALUATE_DIR) -f $(MAKE_FILE_PATH) clean
	@$(MAKE) -C $(MATRIX_OPERATOR_DIR) -f $(MAKE_FILE_PATH) clean	
	@$(MAKE) -C $(MATH_CONCEPT_DIR) -f $(MAKE_FILE_PATH) clean	
	@$(MAKE) -C $(ALGORITHM_DIR) -f $(MAKE_FILE_PATH) clean	
	@$(MAKE) -C $(PARALLELISM_DIR) -f $(MAKE_FILE_PATH) clean	
	@$(MAKE) -C $(IO_DIR) -f $(MAKE_FILE_PATH) clean	
	@$(MAKE) -C $(EVALUATE_DIR) -f $(MAKE_FILE_PATH) clean	

