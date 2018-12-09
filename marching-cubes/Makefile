# Partie configurable
TARGET  := marching-cubes
BUILD   := build
SOURCES := lib src
INCLUDE := lib

CXXFLAGS = -Wall -Wextra -O2 -g -std=c++11
CPPFLAGS = $(foreach dir,$(INCLUDE),-I../$(dir))
LDFLAGS  = -lglut -lGLEW -lGL -pthread
##############################################################################
.SUFFIXES:
.SECONDARY:
.PHONY: clean
# ----------------------------------------------------------------------------
%.o: %.cpp
	@echo '[1m[[32mC++[37m][0m' $(notdir $<)
	@$(CXX) -MMD -MP -MF $(DEPSDIR)/$*.d $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@
# ----------------------------------------------------------------------------
ifneq ($(BUILD),$(notdir $(CURDIR)))
export OUTPUT  := $(CURDIR)/$(BUILD)/$(TARGET)
export VPATH   := $(foreach dir,$(SOURCES),$(CURDIR)/$(dir))
export DEPSDIR := $(CURDIR)/$(BUILD)
CCFILES        := $(foreach dir,$(SOURCES),$(sort $(notdir $(wildcard $(dir)/*.cpp))))
export LD      := $(CXX)
export OFILES  := $(CCFILES:.cpp=.o)
export OUTPUT  := $(CURDIR)/$(TARGET)
.PHONY: $(BUILD) clean

$(BUILD):
	@[ -d $@ ] || mkdir -p $@
	@$(MAKE) --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile

clean:
	@echo clean...
	@rm -fr $(BUILD) $(OUTPUT)
else
# main targets
$(OUTPUT): $(OFILES)
	@echo '[1m[[35mLD[37m][0m' $(notdir $@)
	@$(LD) $^ -o $@ $(LDFLAGS)

-include $(DEPSDIR)/*.d
endif

