CURRENT_MAKE_DIR := $(CURDIR)

GMP_DIR := $(CURRENT_MAKE_DIR)/gmp-6.0.0
MPFR_DIR := $(CURRENT_MAKE_DIR)/mpfr-3.1.3
M4_DIR := $(CURRENT_MAKE_DIR)/m4-1.4.1

GMP_OUT_DIR := $(CURRENT_MAKE_DIR)/gmp
MPFR_OUT_DIR := $(CURRENT_MAKE_DIR)/mpfr
M4_OUT_DIR := $(CURRENT_MAKE_DIR)/m4

MAKE_FILE_PATH = makefile

all: gmp mpfr

gmp: m4
	./configure.sh gmp
	$(MAKE) -C $(GMP_DIR)
	$(MAKE) -C $(GMP_DIR) install
	
mpfr: 
	./configure.sh mpfr
	$(MAKE) -C $(MPFR_DIR)
	$(MAKE) -C $(MPFR_DIR) install
	
m4:
	./configure.sh m4
	$(MAKE) -C $(M4_DIR)
	$(MAKE) -C $(M4_DIR) install

.PHONY: clean

clean:
	@$(MAKE) -C $(GMP_DIR) distclean	
	rm -rf $(GMP_OUT_DIR)
	@$(MAKE) -C $(M4_DIR) distclean
	rm -rf $(M4_OUT_DIR)
	@$(MAKE) -C $(MPFR_DIR) distclean
	rm -rf $(MPFR_OUT_DIR)
