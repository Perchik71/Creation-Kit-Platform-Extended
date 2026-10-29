#pragma once

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			template <class T>
			class BSTSingletonImplicit
			{
			public:
				using value_type = T;
			};

			template <class T>
			class BSTSingletonExplicit
			{
			public:
				using value_type = T;
			};

			template <class T>
			class BSTSingletonSDMOpStaticBuffer
			{
			public:
				using value_type = T;
			};

			template <class Traits>
			class BSTSingletonSDMBase :
				public Traits,
				public BSTSingletonSDMOpStaticBuffer<typename Traits::value_type>
			{
			public:
			};

			template <class T, class Buffer>
			class BSTSDMTraits
			{
			public:
				using value_type = T;
			};

#ifdef __EDG__
			template <class T, template <class> class Buffer = BSTSingletonSDMOpStaticBuffer>
			class BSTSingletonSDM
			{
			public:
				std::uint8_t padding;
			};
#else
			template <class T, template <class> class Buffer = BSTSingletonSDMOpStaticBuffer>
			class BSTSingletonSDM :
				public BSTSingletonSDMBase<BSTSDMTraits<T, Buffer<T>>>
			{
			public:
				using value_type = T;
			};
#endif
		}
	}
}
