# Makefile for libusb based examples
BTSTACK_ROOT ?= /home/yangn0/btstack/

PGM=${ARCH}/app.exe

# optional managers required
MANAGERS=all

CORE += \
	hci_transport_h5.c \
	btstack_worker_h5.c \
	btstack_chipset_cc256x.c \
	btstack_link_key_db_tlv.c \
	btstack_run_loop_posix.c \
    btstack_audio_portaudio.c \
	btstack_tlv_posix.c \
	btstack_uart_posix.c \
	btstack_slip.c \
	hci_dump_posix_fs.c \
	le_device_db_tlv.c \
	rijndael.c \
	btstack_stdin_posix.c \
	btstack_signal.c \
	wav_util.c 					\
	btstack_resample.c \
	bluetooth_init_cc2564B_1.8_BT_Spec_4.1.c \
# bluetooth_init_cc2564C_1.5.c \
# bluetooth_init_cc2564B_1.8_BT_Spec_4.1.c \
# bluetooth_init_cc2564_2.14.c \
	
# TI-WL183x requires TIInit_11.8.32.c

AVRCP += \
	# avrcp.c							\
	# avrcp_controller.c				\
	# avrcp_target.c					\
	# avrcp_browsing.c				\
	# avrcp_browsing_controller.c		\
	# avrcp_browsing_target.c			\
	# avrcp_media_item_iterator.c		\

CLASSIC += \
	sdp_util.c	                \
	gatt_sdp.c                  \
	spp_server.c  				\
	rfcomm.c	                \
	bnep.c	                    \
	sdp_server.c			    \
	device_id_server.c          \
	hfp_hf.c \
	hfp_ag.c \
	hfp_gsm_model.c \
	sco_demo_util.c \
	hfp.c \
	avrcp_cover_art_client.c \
	goep_client.c \
	obex_message_builder.c \
    obex_parser.c \
	hid_host.c \
	btstack_hid_parser.c \
	hid_device.c \
	hsp_hs.c \
	hsp_ag.c


AVDTP += \
	# avdtp_util.c           \
	# avdtp.c                \
	# avdtp_initiator.c      \
	# avdtp_acceptor.c       \
	# avdtp_source.c 	       \
	# avdtp_sink.c           \
	# a2dp.c                 \
	# a2dp_source.c          \
	# a2dp_sink.c            \
	# btstack_ring_buffer.c \

SDP_CLIENT += \
	# sdp_client.o		        \
	# sdp_client_rfcomm.o		    \
	
SBC_DECODER += \
	# btstack_sbc_plc.c \
	# btstack_sbc_decoder_bluedroid.c \

# RTEMS
include ${RTEMS_MAKEFILE_PATH}/Makefile.inc

include $(RTEMS_CUSTOM)

include $(PROJECT_ROOT)/make/leaf.cfg


# btstack
include ${BTSTACK_ROOT}/example/Makefile.inc
include ${BTSTACK_ROOT}/chipset/cc256x/Makefile.inc

CFLAGS  += -g -Wall -Werror \
	-I$(BTSTACK_ROOT)/platform/embedded \
	-I$(BTSTACK_ROOT)/platform/posix \
	-I$(BTSTACK_ROOT)/chipset/cc256x \
    -I${BTSTACK_ROOT}/3rd-party/rijndael \
    -I${BTSTACK_ROOT}/3rd-party/tinydir \

VPATH += ${BTSTACK_ROOT}/3rd-party/rijndael

VPATH += ${BTSTACK_ROOT}/platform/posix
VPATH += ${BTSTACK_ROOT}/platform/embedded

VPATH += ${BTSTACK_ROOT}/chipset/cc256x


# C source names
CORE +=  main.c #main.c init.c 

# MYEXAMPLES = gap_inquiry.c
# MYEXAMPLES = spp_counter.c
# MYEXAMPLES = a2dp_source_demo.c
# MYEXAMPLES = a2dp_sink_demo.c
# MYEXAMPLES = hfp_hf_demo.c
# MYEXAMPLES = hfp_ag_demo.c
# MYEXAMPLES = hid_host_demo.c
# MYEXAMPLES = hid_keyboard_demo.c
# MYEXAMPLES = hid_mouse_demo.c
# MYEXAMPLES = hsp_hs_demo.c

CSRCS = $(CORE) \
		$(AVRCP) \
		$(AVDTP) \
		$(CLASSIC) \
		$(SDP_CLIENT) \
		$(SBC_DECODER) \
		$(SBC_ENCODER) \
		$(HXCMOD_PLAYER) \
		$(CVSD_PLC) \
		$(COMMON) \
		$(GATT_SERVER) \
		$(GATT_CLIENT) \
		$(ATT) \
		$(MYEXAMPLES) \

COBJS = $(CSRCS:%.c=${ARCH}/%.o)

OBJS= $(COBJS) $(CXXOBJS) $(ASOBJS)

LD_LIBS += -lbsd -lrtemstest

all:    ${ARCH} $(PGM)


$(PGM): $(OBJS) 
	$(make-exe)