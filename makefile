MAKE_DIR = $(PWD)

MATH_CONCEPT_DIR := $(MAKE_DIR)/ModuleMathConcepts/Debug
MATRIX_OPERATOR_DIR := $(MAKE_DIR)/ModuleMatrixOperator/Debug
IO_DIR := $(MAKE_DIR)/ModuleIO/Debug
ALGORITHM_DIR := $(MAKE_DIR)/ModuleAlgorithm/Debug
EVALUATE_DIR := $(MAKE_DIR)/ModuleEvaluate/Debug
TEST_MATRIX_OPERATOR_DIR := $(MAKE_DIR)/TestMatrixOperator/Debug
TEST_EVALUATE_DIR := $(MAKE_DIR)/TestEvaluator/Debug

MAKE_FILE_PATH = makefile

all: testMatrixOperator testEvaluate

testMatrixOperator: mathConcept matrixOperator
	@$(MAKE) -C $(TEST_MATRIX_OPERATOR_DIR) -f $(MAKE_FILE_PATH) all	

mathConcept: 
	@$(MAKE) -C $(MATH_CONCEPT_DIR) -f $(MAKE_FILE_PATH) all	

matrixOperator:
	@$(MAKE) -C $(MATRIX_OPERATOR_DIR) -f $(MAKE_FILE_PATH) all	

testEvaluate: io algorithm evaluate
	@$(MAKE) -C $(TEST_EVALUATE_DIR) -f $(MAKE_FILE_PATH) all	

io:
	@$(MAKE) -C $(IO_DIR) -f $(MAKE_FILE_PATH) all

evaluate:
	@$(MAKE) -C $(EVALUATE_DIR) -f $(MAKE_FILE_PATH) all	

algorithm:
	@$(MAKE) -C $(ALGORITHM_DIR) -f $(MAKE_FILE_PATH) all	

.PHONY: clean

clean:
	@$(MAKE) -C $(TEST_MATRIX_OPERATOR_DIR) -f $(MAKE_FILE_PATH) clean	
	@$(MAKE) -C $(TEST_EVALUATE_DIR) -f $(MAKE_FILE_PATH) clean
	@$(MAKE) -C $(MATRIX_OPERATOR_DIR) -f $(MAKE_FILE_PATH) clean	
	@$(MAKE) -C $(MATH_CONCEPT_DIR) -f $(MAKE_FILE_PATH) clean	
	@$(MAKE) -C $(ALGORITHM_DIR) -f $(MAKE_FILE_PATH) clean	
	@$(MAKE) -C $(IO_DIR) -f $(MAKE_FILE_PATH) clean	
	@$(MAKE) -C $(EVALUATE_DIR) -f $(MAKE_FILE_PATH) clean	

