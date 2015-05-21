MAKE_DIR = $(PWD
)
MATH_CONCEPT_DIR := $(MAKE_DIR)/ModuleMathConcepts
MATRIX_OPERATOR_DIR := $(MAKE_DIR)/ModuleMatrixOperator
IO_DIR := $(MAKE_DIR)/ModuleIO
ALGORITHM_DIR := $(MAKE_DIR)/ModuleAlgorithm
EVALUATE_DIR := $(MAKE_DIR)/ModuleEvaluate
TEST_MATRIX_OPERATOR_DIR := $(MAKE_DIR)/TestMatrixOperator
TEST_EVALUATE_DIR := $(MAKE_DIR)/TestEvaluator

MAKE_FILE_PATH = Debug/makefile

all: testMatrixOperator testEvaluate

testMatrixOperator: mathConcept matrixOperator
	@$(MAKE) -f $(TEST_MATRIX_OPERATOR_DIR)/$(MAKE_FILE_PATH) all	

mathConcept: 
	@$(MAKE) -f $(MATH_CONCEPT_DIR)/$(MAKE_FILE_PATH) all	

matrixOperator:
	@$(MAKE) -f $(MATRIX_OPERATOR_DIR)/$(MAKE_FILE_PATH) all	

testEvaluate: io algorithm evaluate
	@$(MAKE) -f $(TEST_EVALUATE_DIR)/$(MAKE_FILE_PATH) all	

io:
	@$(MAKE) -f $(IO_DIR)/$(MAKE_FILE_PATH)	all

evaluate:
	@$(MAKE) -f $(EVALUATE_DIR)/$(MAKE_FILE_PATH) all	

algorithm:
	@$(MAKE) -f $(ALGORITHM_DIR)/$(MAKE_FILE_PATH) all	

.PHONY: clean

clean:
	@$(MAKE) -f $(TEST_MATRIX_OPERATOR_DIR)/$(MAKE_FILE_PATH) clean	
	@$(MAKE) -f $(TEST_EVALUATE_DIR)/$(MAKE_FILE_PATH) clean
	@$(MAKE) -f $(MATRIX_OPERATOR_DIR)/$(MAKE_FILE_PATH) clean	
	@$(MAKE) -f $(MATH_CONCEPT_DIR)/$(MAKE_FILE_PATH) clean	
	@$(MAKE) -f $(ALGORITHM_DIR)/$(MAKE_FILE_PATH) clean	
	@$(MAKE) -f $(IO_DIR)/$(MAKE_FILE_PATH) clean	
	@$(MAKE) -f $(EVALUATE_DIR)/$(MAKE_FILE_PATH) clean	








