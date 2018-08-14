PROTOC = ../../../3rd/protobuf/bin/protoc

BUILD = ../proto_pb
	
PROTOSRCS = battle.proto 

PROTO :=

define PROTO_temp
  	TAR :=  $(BUILD)/$(notdir $(basename $(1)))
  	PROTO := $(PROTO) $$(TAR).pb
  	$$(TAR).pb : | $(BUILD)
  	$$(TAR).pb : $(1)
		$(PROTOC) -o$$@ $$<
endef

$(foreach s,$(PROTOSRCS),$(eval $(call PROTO_temp,$(s))))

all : $(PROTO) proto

proto:
	$(PROTOC) --cpp_out=../proto_cpp *.proto
