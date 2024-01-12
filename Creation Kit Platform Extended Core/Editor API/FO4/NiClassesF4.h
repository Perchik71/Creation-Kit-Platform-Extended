//////////////////////////////////////////
/*
* Fallout 4 Script Extender (F4SE)
* by Ian Patterson, Stephen Abel, and Brendan Borthwick (ianpatt, behippo, and purplelunchbox)
* 
* Contact the F4SE Team
*
* Entire Team
* Send email to team [at] f4se [dot] silverlock [dot] org
*
* Ian (ianpatt)
* Send email to ianpatt+f4se [at] gmail [dot] com
*
* Stephen (behippo)
* Send email to gamer [at] silverlock [dot] org
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
* PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
* FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
* OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
* DEALINGS IN THE SOFTWARE.
*/
//////////////////////////////////////////

//////////////////////////////////////////
/*
* Copyright (c) 2022 Perchik71 <email:perchik71@outlook.com>
*
* Permission is hereby granted, free of charge, to any person obtaining a copy of this
* software and associated documentation files (the "Software"), to deal in the Software
* without restriction, including without limitation the rights to use, copy, modify, merge,
* publish, distribute, sublicense, and/or sell copies of the Software, and to permit
* persons to whom the Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all copies or
* substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
* PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
* FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
* OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
* DEALINGS IN THE SOFTWARE.
*/
//////////////////////////////////////////

#pragma once
#pragma pack(push, 4)

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace Fallout4
		{
			class NiBinaryStream 
			{
			public:
				virtual ~NiBinaryStream() = default;
			public:
				virtual bool IsValid(VOID) = 0;
			};

			class NiFile : public NiBinaryStream 
			{
			public:
				virtual ~NiFile() = default;
			public:
				enum FileModes {
					kFileMode_ReadOnly = 0,
					kFileMode_WriteOnly = 1,
					kFileMode_AppendOnly = 2,
				};
			protected:
				// members
				/*000*/ // --vtbls
				/*008*/ uint64_t _Pos;				// init to 0
				/*010*/ uint64_t _BufferAllocSize;	// init to 0x800	(sent for reading/writing)
				/*018*/ uint64_t _BufferReadSize;	// init to 0		(answered by the function)
				/*020*/ uint64_t _BufferOffset;		// init to 0
				/*028*/ uint64_t _SizeBuffer;		// init to 0
				/*030*/ void* _Buffer;				// allocated/deallocated on form heap by constructor/destructor
				/*038*/ uint64_t _dwUnk38;
				/*040*/ HANDLE _FileHandle;
				/*048*/ char _FileName[MAX_PATH];
				/*14C*/ bool _Good;					// true if file is exist or no error
			public:
				inline uint64_t GetPosition(VOID) const { return _Pos; }
				inline uint64_t GetOffsetOfTheBuffer(VOID) const { return _BufferOffset; }
				inline uint64_t GetSizeBuffer(VOID) const { return _SizeBuffer; }
				inline HANDLE GetHandle(VOID) const { return _FileHandle; }
				inline LPCSTR GetFileName(VOID) const { return _FileName; }
				inline bool IsGood(VOID) const { return _Good; }
			public:
				virtual bool IsValid(VOID) { return IsGood(); }
			};

			// 04 or 08 depending alignment
			struct BSIntrusiveRefCounted
			{
			public:
				volatile uint32_t m_refCount;	// 00
			};

			// 04
			struct BSNonReentrantSpinLock 
			{
			public:
				volatile uint32_t uiLock;		// 00
			};

			class SimpleLock
			{
				enum { kFastSpinThreshold = 10000 };

				volatile uint32_t threadID;		// 00
				volatile uint32_t lockCount;	// 04

			public:
				SimpleLock() : threadID(0), lockCount(0) {}

				void Lock(uint32_t pauseAttempts = 0);
				void Release();
			};

			// 08
			class BSReadWriteLock 
			{
				enum {
					kFastSpinThreshold = 10000,
					kLockWrite = 0x80000000,
					kLockCountMask = 0xFFFFFFF
				};

				volatile uint32_t threadID;		// 00
				volatile uint32_t lockValue;	// 04
			public:
				BSReadWriteLock() : threadID(0), lockValue(0) {}

				void LockForReadAndWrite();
				bool TryLockForWrite();
				bool TryLockForRead();
				void Unlock();
			};

			class BSReadLocker 
			{
			public:
				BSReadLocker(BSReadWriteLock* lock) { m_lock = lock; m_lock->TryLockForRead(); }
				~BSReadLocker() { m_lock->Unlock(); }
			protected:
				BSReadWriteLock* m_lock;
			};

			class BSWriteLocker
			{
			public:
				BSWriteLocker(BSReadWriteLock* lock) { m_lock = lock; m_lock->TryLockForWrite(); }
				~BSWriteLocker() { m_lock->Unlock(); }

			protected:
				BSReadWriteLock* m_lock;
			};

			class BSReadAndWriteLocker 
			{
			public:
				BSReadAndWriteLocker(BSReadWriteLock* lock) { m_lock = lock; m_lock->LockForReadAndWrite(); }
				~BSReadAndWriteLocker() { m_lock->Unlock(); }

			protected:
				BSReadWriteLock* m_lock;
			};

			class SimpleLocker
			{
			public:
				SimpleLocker(SimpleLock* dataHolder) { m_lock = dataHolder; m_lock->Lock(); }
				~SimpleLocker() { m_lock->Release(); }

			protected:
				SimpleLock* m_lock;
			};

			template <typename T>
			class SafeDataHolder
			{
			protected:
				SimpleLock m_lock;
			public:
				T m_data;
			public:
				inline void Lock() { m_lock.Lock(); }
				inline void Release() { m_lock.Release(); }
			};
		}
	}
}

#pragma pack(pop)