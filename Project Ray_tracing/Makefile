#Makefile original de David Odin

.SUFFIXES:
.SECONDARY:

TARGET  := pgm
BUILD   := build


######################################################################################
CXXFLAGS += -g -Wall -Wextra -O2 -std=c++11 -DCORRECTION -Wno-comment -Wno-unused-function -Wno-unused-parameter -fopenmp
INCLUDE += -I../src/
LDFLAGS += -ldl -fopenmp

ifneq ($(BUILD),$(notdir $(CURDIR)))
#---------------------------------------------------------------------------------
SOURCES := $(shell find src/ -type d)
export OUTPUT   :=      $(CURDIR)/$(BUILD)/$(TARGET)
export VPATH    :=      $(foreach dir,$(SOURCES),$(CURDIR)/$(dir))
export DEPSDIR  :=      $(CURDIR)/$(BUILD)
CXXFILES        :=      $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))

export LD       :=      $(CXX)
export OFILES   :=      $(CXXFILES:.cpp=.o)

export OUTPUT   :=      $(CURDIR)/$(TARGET)
.PHONY: $(BUILD) clean

#---------------------------------------------------------------------------------
$(BUILD):
	@[ -d $@ ] || mkdir -p $@
	@$(MAKE) --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile

#---------------------------------------------------------------------------------
clean:
	@echo clean ...
	@rm -fr $(BUILD) $(OUTPUT)
#---------------------------------------------------------------------------------
else

# main targets
$(OUTPUT): $(OFILES)
	@echo '[1m[[35mLD[37m][0m' $(notdir $@)
	@$(LD) $^ -o $@ $(LIBPATHS) $(LDFLAGS)

-include $(DEPSDIR)/*.d
endif
##################################################################################
%.o: %.cpp
	@echo '[1m[[32mCC[37m][0m' $(notdir $<)
	@$(CXX) -MMD -MP -MF $(DEPSDIR)/$*.d $(CXXFLAGS) $(CPPFLAGS) $(INCLUDE) -c $< -o $@

