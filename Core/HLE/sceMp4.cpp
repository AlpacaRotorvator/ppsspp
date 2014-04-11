// Copyright (c) 2012- PPSSPP Project.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, version 2.0 or later versions.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License 2.0 for more details.

// A copy of the GPL 2.0 should have been included with the program.
// If not, see http://www.gnu.org/licenses/

// Official git repository and contact information can be found at
// https://github.com/hrydgard/ppsspp and http://www.ppsspp.org/.

#include "Core/HLE/HLE.h"
#include "Core/HLE/FunctionWrappers.h"
#include "Core/Reporting.h"
#include "Core/HLE/sceMp4.h"
#include "Core/HW/SimpleAudioDec.h"


class AACCtx{
public:
	// aac source informations
	u64 startPos;
	u64 endPos;
	u32 AACBuf;
	u32 AACBufSize;
	u32 PCMBuf;
	u32 PCMBufSize;
	int freq;

	// audio settings
	u32 SumDecodedSamples;
	int LoopNum;
	int Channels;
	u32 MaxOutputSample;

	// aac decoder
	SimpleAudio *decoder;

	// buffers informations
	int aacBufAvailable; // the available buffer of AACBuf to be able to recharge data
	int readPos; // read position in audio source file
	int writePos; // write position in AACBuf, i.e. the size of bytes decoded in AACBuf.

	AACCtx() :decoder(NULL){};
	~AACCtx(){
		if (decoder){
			AudioClose(&decoder);
			decoder = NULL;
		}
	};

	void DoState(PointerWrap &p) {
		auto s = p.Section("AACContext", 1);
		if (!s)
			return;

		p.Do(startPos);
		p.Do(endPos);
		p.Do(AACBuf);
		p.Do(AACBufSize);
		p.Do(PCMBuf);
		p.Do(PCMBufSize);
		p.Do(freq);
		p.Do(SumDecodedSamples);
		p.Do(LoopNum);
		p.Do(Channels);
		p.Do(MaxOutputSample);
		p.Do(aacBufAvailable);
		p.Do(readPos);
		p.Do(writePos);

		if (p.mode == p.MODE_READ){
			decoder = new SimpleAudio(PSP_CODEC_AAC);
		}
	};
};

static std::map<u32, AACCtx*> aacMap;

AACCtx *getAacCtx(u32 id) {
	if (aacMap.find(id) == aacMap.end())
		return NULL;
	return aacMap[id];
}

void __AACShutdown() {
	for (auto it = aacMap.begin(), end = aacMap.end(); it != end; it++) {
		delete it->second;
	}
	aacMap.clear();
}

void __AACDoState(PointerWrap &p) {
	auto s = p.Section("sceAAC", 0, 1);
	if (!s)
		return;

	p.Do(aacMap);
}

u32 sceMp4Init()
{
	INFO_LOG(ME, "sceMp4Init()");
	return 0;
}

u32 sceMp4Finish()
{
	ERROR_LOG(ME, "UNIMPL sceMp4Finish()");
	return 0;
}

u32 sceMp4Create()
{
	ERROR_LOG_REPORT(ME, "UNIMPL sceMp4Create()");
	return 0;
}

u32 sceMp4GetNumberOfSpecificTrack()
{
	ERROR_LOG(ME, "UNIMPL sceMp4GetNumberOfSpecificTrack()");
	return 1;
}

u32 sceMp4GetMovieInfo()
{
	ERROR_LOG(ME, "UNIMPL sceMp4GetMovieInfo()");
	return 0;
}

u32 sceMp4CreatesceMp4GetNumberOfMetaData()
{
	ERROR_LOG(ME, "UNIMPL sceMp4GetNumberOfMetaData()");
	return 0;
}

u32 sceMp4Delete()
{
	ERROR_LOG(ME, "UNIMPL sceMp4Delete()");
	return 0;
}

u32 sceMp4AacDecodeInitResource()
{
	ERROR_LOG(ME, "UNIMPL sceMp4AacDecodeInitResource()");
	return 0;
}

u32 sceMp4GetAvcTrackInfoData()
{
	ERROR_LOG(ME, "UNIMPL sceMp4GetAvcTrackInfoData()");
	return 0;
}

u32 sceMp4TrackSampleBufConstruct()
{
	ERROR_LOG(ME, "UNIMPL sceMp4TrackSampleBufConstruct()");
	return 0;
}

u32 sceMp4TrackSampleBufQueryMemSize()
{
	ERROR_LOG(ME, "UNIMPL sceMp4TrackSampleBufQueryMemSize()");
	return 0;
}

u32 sceMp4GetSampleInfo()
{
	ERROR_LOG(ME, "UNIMPL sceMp4GetSampleInfo()");
	return 0;
}

u32 sceMp4GetSampleNumWithTimeStamp()
{
	ERROR_LOG(ME, "UNIMPL sceMp4GetSampleNumWithTimeStamp()");
	return 0;
}

u32 sceMp4TrackSampleBufFlush()
{
	ERROR_LOG(ME, "UNIMPL sceMp4TrackSampleBufFlush()");
	return 0;
}

u32 sceMp4AacDecodeInit()
{
	ERROR_LOG(ME, "UNIMPL sceMp4AacDecodeInit()");
	return 0;
}

u32 sceMp4GetAacTrackInfoData()
{
	ERROR_LOG(ME, "UNIMPL sceMp4GetAacTrackInfoData()");
	return 0;
}

u32 sceMp4GetNumberOfMetaData()
{
	ERROR_LOG(ME, "UNIMPL sceMp4GetNumberOfMetaData()");
	return 0;
}

u32 sceMp4RegistTrack()
{
	ERROR_LOG(ME, "UNIMPL sceMp4RegistTrack()");
	return 0;
}

u32 sceMp4SearchSyncSampleNum()
{
	ERROR_LOG(ME, "UNIMPL sceMp4SearchSyncSampleNum()");
	return 0;
}


// sceAac module starts from here

u32 sceAacExit(u32 id)
{
	INFO_LOG(ME, "sceAacExit(id %i)", id);
	if (aacMap.find(id) != aacMap.end()) {
		delete aacMap[id];
		aacMap.erase(id);
	}
	else{
		ERROR_LOG(ME, "%s: bad aac id %08x", __FUNCTION__, id);
		return -1;
	}
	return 0;
}

u32 sceAacInit(u32 id)
{
	INFO_LOG(ME, "UNIMPL sceAacInit(%08x)", id);
	if (!Memory::IsValidAddress(id)){
		ERROR_LOG(ME, "sceAacInit() AAC Invalid id address %08x", id);
		return ERROR_AAC_INVALID_ADDRESS;
	}
	AACCtx *aac = new AACCtx;
	aac->startPos = Memory::Read_U64(id);				// Audio stream start position.
	aac->endPos = Memory::Read_U32(id + 8);				// Audio stream end position.
	aac->AACBuf = Memory::Read_U32(id + 16);            // Input AAC data buffer.	
	aac->AACBufSize = Memory::Read_U32(id + 20);        // Input AAC data buffer size.
	aac->PCMBuf = Memory::Read_U32(id + 24);            // Output PCM data buffer.
	aac->PCMBufSize = Memory::Read_U32(id + 28);        // Output PCM data buffer size.
	aac->freq = Memory::Read_U32(id + 32);              // Frequency.
	if (aac->AACBuf == 0 || aac->PCMBuf == 0) {
		ERROR_LOG(ME, "sceAacInit() AAC INVALID ADDRESS %08x", aac->AACBuf);
		delete aac;
		return ERROR_AAC_INVALID_ADDRESS;
	}
	if (aac->startPos < 0 || aac->startPos > aac->endPos) {
		ERROR_LOG(ME, "sceAacInit() AAC INVALID startPos %i", aac->startPos);
		delete aac;
		return ERROR_AAC_INVALID_PARAMETER;
	}
	if (aac->AACBufSize < 8192 || aac->PCMBufSize < 8192) {
		ERROR_LOG(ME, "sceAacInit() AAC INVALID PARAMETER, bufferSize %i outputSize %i reserved %i", aac->AACBufSize, aac->PCMBufSize);
		delete aac; 
		return ERROR_AAC_INVALID_PARAMETER;
	}
	if (aac->freq != 24000 && aac->freq != 32000 && aac->freq != 44100 && aac->freq != 48000) {
		ERROR_LOG(ME, "sceAacInit() AAC INVALID freq %i", aac->freq);
		delete aac;
		return ERROR_AAC_INVALID_PARAMETER;
	}

	DEBUG_LOG(ME, "startPos %x endPos %x AACbuf %08x AACbufSize %08x PCMbuf %08x PCMbufSize %08x freq %d", 
		aac->startPos, aac->endPos, aac->AACBuf, aac->AACBufSize, aac->PCMBuf, aac->PCMBufSize, aac->freq);

	aac->Channels = 2;
	aac->SumDecodedSamples = 0;
	aac->MaxOutputSample = aac->PCMBufSize / 4;
	aac->LoopNum = -1;
	aac->aacBufAvailable = 0;
	aac->MaxOutputSample = 0;
	aac->readPos = aac->startPos;

	// create aac decoder
	aac->decoder = new SimpleAudio(PSP_CODEC_AAC);

	// close the audio if id already exist.
	sceAacExit(id);

	aacMap[id] = aac;

	return id;
}

u32 sceAacInitResource(u32 numberIds)
{
	// Do nothing here
	INFO_LOG_REPORT(ME, "sceAacInitResource(%i)", numberIds);
	return 0;
}

u32 sceAacTermResource()
{
	ERROR_LOG(ME, "UNIMPL sceAacTermResource()");
	return 0;
}

u32 sceAacDecode(u32 id, u32 pcmAddr)
{
	// return the size of output pcm
	DEBUG_LOG(ME, "sceAacDecode(id %i, bufferAddress %08x)", id, pcmAddr);
	auto ctx = getAacCtx(id);
	if (!ctx) {
		ERROR_LOG(ME, "%s: bad aac id %08x", __FUNCTION__, id);
		return -1;
	}
	if (!Memory::IsValidAddress(pcmAddr)){
		ERROR_LOG(ME, "%s: output bufferAddress %08x is invalid", __FUNCTION__, pcmAddr);
		return -1;
	}

	auto inbuff = Memory::GetPointer(ctx->AACBuf);
	auto outbuf = Memory::GetPointer(ctx->PCMBuf);
	memset(outbuf, 0, ctx->PCMBufSize);
	u32 outpcmbufsize = 0;

	// move inbuff to writePos of buffer
	inbuff += ctx->writePos;

	// decode frames in aacBuf and output into PCMBuf if it is not exceed
	while (ctx->aacBufAvailable > 0 && outpcmbufsize < ctx->PCMBufSize){
		int pcmframesize;
		// decode
		ctx->decoder->Decode(inbuff, ctx->aacBufAvailable, outbuf, &pcmframesize);
		if (pcmframesize == 0){
			// no output pcm, we have either no data or no enough data to decode
			// move back audio source readPos to the begin of the last incomplete frame if we not start looping and reset available aacBuf
			if (ctx->readPos > ctx->startPos) { // this means we are not begin to loop yet
				ctx->readPos -= ctx->aacBufAvailable;
			}
			ctx->aacBufAvailable = 0;
			break;
		}
		// count total output pcm size 
		outpcmbufsize += pcmframesize;
		// count total output samples
		ctx->SumDecodedSamples += ctx->decoder->getOutSamples();
		// move inbuff position to next frame
		int srcPos = ctx->decoder->getSourcePos();
		inbuff += srcPos;
		// decrease available aacBuf
		ctx->aacBufAvailable -= srcPos;
		// modify the writePos value
		ctx->writePos += srcPos;
		// move outbuff position to the current end of output 
		outbuf += pcmframesize;
	}

	Memory::Write_U32(ctx->PCMBuf, pcmAddr);

	return outpcmbufsize;
}

u32 sceAacGetLoopNum(u32 id)
{
	INFO_LOG(ME, "sceAacGetLoopNum(id %i)", id);
	auto ctx = getAacCtx(id);
	if (!ctx) {
		ERROR_LOG(ME, "%s: bad aac id %08x", __FUNCTION__, id);
		return -1;
	}
	return ctx->LoopNum;
}

u32 sceAacSetLoopNum(u32 id, int loop)
{
	INFO_LOG(ME, "sceAacSetLoopNum(id %i,loop %d)", id, loop);
	auto ctx = getAacCtx(id);
	if (!ctx) {
		ERROR_LOG(ME, "%s: bad aac id %08x", __FUNCTION__, id);
		return -1;
	}

	ctx->LoopNum = loop;
	return 0;
}

int sceAacCheckStreamDataNeeded(u32 id)
{
	// return 1 to read more data stream, 0 don't read, <0 error
	DEBUG_LOG(ME, "sceAacCheckStreamDataNeeded(%i)", id);

	auto ctx = getAacCtx(id);
	if (!ctx) {
		ERROR_LOG(ME, "%s: bad aac id %08x", __FUNCTION__, id);
		return -1;
	}

	// if we have no available aac buffer, and the current read position in source file is not the end of stream, then we can read
	if (ctx->aacBufAvailable == 0 && ctx->readPos < ctx->endPos){
		return 1;
	}
	return 0;
}

u32 sceAacNotifyAddStreamData(u32 id, int size)
{
	// check how many bytes we have read from source file
	DEBUG_LOG(ME, "sceAacNotifyAddStreamData(%i, %08x)", id, size);

	auto ctx = getAacCtx(id);
	if (!ctx) {
		ERROR_LOG(ME, "%s: bad aac id %08x", __FUNCTION__, id);
		return -1;
	}

	ctx->readPos += size;
	ctx->aacBufAvailable += size;
	ctx->writePos = 0;

	if (ctx->readPos >= ctx->endPos && ctx->LoopNum != 0){
		// if we need loop, reset readPos
		ctx->readPos = ctx->startPos;
		// reset LoopNum
		if (ctx->LoopNum > 0){
			ctx->LoopNum--;
		}
	}

	return 0;
}

u32 sceAacGetInfoToAddStreamData(u32 id, u32 buff, u32 size, u32 srcPos)
{
	// read from stream position srcPos of size bytes into buff
	DEBUG_LOG(ME, "sceAacGetInfoToAddStreamData(%08X, %08X, %08X, %08X)", id, buff, size, srcPos);

	auto ctx = getAacCtx(id);
	if (!ctx) {
		ERROR_LOG(ME, "%s: bad aac handle %08x", __FUNCTION__, id);
		return -1;
	}

	// we can recharge aacBuf from its begining
	if (Memory::IsValidAddress(buff))
		Memory::Write_U32(ctx->AACBuf, buff);
	if (Memory::IsValidAddress(size))
		Memory::Write_U32(ctx->AACBufSize, size);
	if (Memory::IsValidAddress(srcPos))
		Memory::Write_U32(ctx->readPos, srcPos);

	return 0;
}

u32 sceAacGetMaxOutputSample(u32 id)
{
	DEBUG_LOG(ME, "sceAacGetMaxOutputSample(id %i)", id);
	auto ctx = getAacCtx(id);
	if (!ctx) {
		ERROR_LOG(ME, "%s: bad aac id %08x", __FUNCTION__, id);
		return -1;
	}

	return ctx->MaxOutputSample;
}

u32 sceAacGetSumDecodedSample(u32 id)
{
	DEBUG_LOG(ME, "sceAacGetSumDecodedSample(id %i)", id);
	auto ctx = getAacCtx(id);
	if (!ctx) {
		ERROR_LOG(ME, "%s: bad aac id %08x", __FUNCTION__, id);
		return -1;
	}

	return ctx->SumDecodedSamples;
}

u32 sceAacResetPlayPosition(u32 id)
{
	INFO_LOG(ME, "sceAacResetPlayPosition(id %i)", id);
	auto ctx = getAacCtx(id);
	if (!ctx) {
		ERROR_LOG(ME, "%s: bad aac id %08x", __FUNCTION__, id);
		return -1;
	}
	
	ctx->readPos = ctx->startPos;
	return 0;
}

const HLEFunction sceMp4[] =
{
	{0x68651CBC, WrapU_V<sceMp4Init>, "sceMp4Init"},
	{0x9042B257, WrapU_V<sceMp4Finish>, "sceMp4Finish"},
	{0xB1221EE7, WrapU_V<sceMp4Create>, "sceMp4Create"},
	{0x538C2057, WrapU_V<sceMp4Delete>, "sceMp4Delete"},
	{0x113E9E7B, WrapU_V<sceMp4GetNumberOfMetaData>, "sceMp4GetNumberOfMetaData"},
	{0x7443AF1D, WrapU_V<sceMp4GetMovieInfo>, "sceMp4GetMovieInfo"},
	{0x5EB65F26, WrapU_V<sceMp4GetNumberOfSpecificTrack>, "sceMp4GetNumberOfSpecificTrack"},
	{0x7ADFD01C, WrapU_V<sceMp4RegistTrack>, "sceMp4RegistTrack"},
	{0xBCA9389C, WrapU_V<sceMp4TrackSampleBufQueryMemSize>, "sceMp4TrackSampleBufQueryMemSize"},
	{0x9C8F4FC1, WrapU_V<sceMp4TrackSampleBufConstruct>, "sceMp4TrackSampleBufConstruct"},
	{0x0F0187D2, WrapU_V<sceMp4GetAvcTrackInfoData>, "sceMp4GetAvcTrackInfoData"},
	{0x9CE6F5CF, WrapU_V<sceMp4GetAacTrackInfoData>, "sceMp4GetAacTrackInfoData"},
	{0x4ED4AB1E, WrapU_V<sceMp4AacDecodeInitResource>, "sceMp4AacDecodeInitResource"},
	{0x10EE0D2C, WrapU_V<sceMp4AacDecodeInit>, "sceMp4AacDecodeInit"},
	{0x496E8A65, WrapU_V<sceMp4TrackSampleBufFlush>, "sceMp4TrackSampleBufFlush"},
	{0xB4B400D1, WrapU_V<sceMp4GetSampleNumWithTimeStamp>, "sceMp4GetSampleNumWithTimeStamp"},
	{0xF7C51EC1, WrapU_V<sceMp4GetSampleInfo>, "sceMp4GetSampleInfo"},
	{0x74A1CA3E, WrapU_V<sceMp4SearchSyncSampleNum>, "sceMp4SearchSyncSampleNum"},
	{0xD8250B75, 0, "sceMp4PutSampleNum"},
	{0x8754ECB8, 0, "sceMp4TrackSampleBufAvailableSize"},
	{0x31BCD7E0, 0, "sceMp4TrackSampleBufPut"},
	{0x5601A6F0, 0, "sceMp4GetAacAu"},
	{0x7663CB5C, 0, "sceMp4AacDecode"},
	{0x503A3CBA, 0, "sceMp4GetAvcAu"},
	{0x01C76489, 0, "sceMp4TrackSampleBufDestruct"},
	{0x6710FE77, 0, "sceMp4UnregistTrack"},
	{0x5D72B333, 0, "sceMp4AacDecodeExit"},
	{0x7D332394, 0, "sceMp4AacDecodeTermResource"},
	{0x131BDE57, 0, "sceMp4InitAu"},
	{0x17EAA97D, 0, "sceMp4GetAvcAuWithoutSampleBuf"},
	{0x28CCB940, 0, "sceMp4GetTrackEditList"},
	{0x3069C2B5, 0, "sceMp4GetAvcParamSet"},
	{0xD2AC9A7E, 0, "sceMp4GetMetaData"},
	{0x4FB5B756, 0, "sceMp4GetMetaDataInfo"},
	{0x427BEF7F, 0, "sceMp4GetTrackNumOfEditList"},
	{0x532029B8, 0, "sceMp4GetAacAuWithoutSampleBuf"},
	{0xA6C724DC, 0, "sceMp4GetSampleNum"},
	{0x3C2183C7, 0, "mp4msv_3C2183C7"},
	{0x9CA13D1A, 0, "mp4msv_9CA13D1A"},
};

// 395
const HLEFunction sceAac[] = {
	{0xE0C89ACA, WrapU_U<sceAacInit>, "sceAacInit"},
	{0x33B8C009, WrapU_U<sceAacExit>, "sceAacExit"},
	{0x5CFFC57C, WrapU_U<sceAacInitResource>, "sceAacInitResource"},
	{0x23D35CAE, WrapU_V<sceAacTermResource>, "sceAacTermResource"},
	{0x7E4CFEE4, WrapU_UU<sceAacDecode>, "sceAacDecode"},
	{0x523347D9, WrapU_U<sceAacGetLoopNum>, "sceAacGetLoopNum"},
	{0xBBDD6403, WrapU_UI<sceAacSetLoopNum>, "sceAacSetLoopNum"},
	{0xD7C51541, WrapI_U<sceAacCheckStreamDataNeeded>, "sceAacCheckStreamDataNeeded"},
	{0xAC6DCBE3, WrapU_UI<sceAacNotifyAddStreamData>, "sceAacNotifyAddStreamData"},
	{0x02098C69, WrapU_UUUU<sceAacGetInfoToAddStreamData>, "sceAacGetInfoToAddStreamData"},
	{0x6DC7758A, WrapU_U<sceAacGetMaxOutputSample>, "sceAacGetMaxOutputSample"},
	{0x506BF66C, WrapU_U<sceAacGetSumDecodedSample>, "sceAacGetSumDecodedSample"},
	{0xD2DA2BBA, WrapU_U<sceAacResetPlayPosition>, "sceAacResetPlayPosition"},
};

void Register_sceMp4()
{
	RegisterModule("sceMp4", ARRAY_SIZE(sceMp4), sceMp4);
	RegisterModule("sceAac", ARRAY_SIZE(sceAac), sceAac);
}
