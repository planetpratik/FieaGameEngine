#pragma once
#include "HashMap.h"
#include <string>
#include <gsl/gsl>
namespace FieaGameEngine
{
	/// <summary>
	/// Templated Class for Factory Design Pattern which creates objects by their Typename.
	/// It is suitable for use with any interface class which has a default constructor.
	/// </summary>
	template <typename AbstractProductT>
	class Factory
	{
	public:
		/// <summary>Default Constructor for Factory class. (Defaulted)</summary>
		Factory() = default;

		/// <summary>Destructor for Factory class. (Defaulted)</summary>
		virtual ~Factory() = default;

		/// <summary>Copy Constructor for Factory class. (Deleted)</summary>
		/// <param name="t_rhs">Const reference to passed Factory.</param>
		Factory(const Factory& t_rhs) = delete;

		/// <summary>Copy Assignment operator implementation for Factory class. (Deleted)</summary>
		/// <param name="t_rhs">Const reference to passed Factory.</param>
		/// <returns>Returns Reference to Factory.</returns>
		Factory& operator=(const Factory& t_rhs) = delete;

		/// <summary>Move Constructor for Factory class. (Deleted).</summary>
		/// <param name="t_rhs">R-Value reference to passed Factory which is to be moved.</param>
		Factory(Factory&& t_rhs) = delete;

		/// <summary>Move Assignment operator implementation for Factory class. (Deleted)</summary>
		/// <param name="t_rhs">R-Value reference to passed Factory which is to be moved.</param>
		/// <returns>Returns Reference to Factory.</returns>
		Factory& operator=(Factory&& t_rhs) = delete;

		/// <summary>Finds Concrete Factory from the Concrete Factory table. (Static)</summary>
		/// <param name="t_concrete_factory_name">Const std::string reference to name of associated factory which is to be searched.</param>
		/// <returns>Returns pointer to Factory if found else returns nullptr.</returns>
		static Factory* find(const std::string& t_concrete_factory_name);

		/// <summary>Create Concrete Product associated with Concrete factory class. (Static)</summary>
		/// <param name="t_concrete_factory_name">Const std::string reference to name of associated factory.</param>
		/// <returns>Returns pointer to Abstract Product type.</returns>
		static gsl::owner<AbstractProductT*> create(const std::string& t_concrete_factory_name);

		/// <summary>Get count of no. of elements present in Factory Table. (Static)</summary>
		/// <returns>Returns Factory table's count of elements in Unsigned Integer (32-Bit).</returns>
		static uint32_t getFactoryTableSize();

		/// <summary>Create Concrete Product associated with Concrete factory class. (Pure Virtual Function)</summary>
		/// <returns>Returns pointer to Abstract Product type.</returns>
		virtual gsl::owner<AbstractProductT*> create() = 0;

		/// <summary>Gets name of the product class instantiated by Concrete Factory. (Pure Virtual Function)</summary>
		/// <returns>Returns std::string indicating name of the Product class.</returns>
		virtual const std::string className() const = 0;

	protected:
		/// <summary>Given a reference to a concrete factory, add it to the list of factories for this abstract factory.</summary>
		/// <param name="t_concrete_factory">Reference to a Concrete factory which is to be added in the list.</param>
		static void add(Factory& t_concrete_factory);

		/// <summary>Given a reference to a concrete factory, remove it from the list of factories for this abstract factory.</summary>
		/// <param name="t_concrete_factory">Reference to a Concrete factory which is to be removed from the list.</param>
		static void remove(Factory& t_concrete_factory);

	private:
		static HashMap<std::string, Factory<AbstractProductT>*> m_concrete_factories_table;
	};

	/// <summary>
	/// This Macro generates ConcreteFactory class from given Concrete Product Type and Abstract Product from which it is derived.
	/// To Implement Factory, User needs to write only one line of code as follows.
	/// ConcreteFactory(ConcreteProductType, AbstractProductType)
	/// </summary>
#define ConcreteFactory(ConcreteProductT, AbstractProductT)							\
	class ConcreteProductT##Factory final: public Factory<AbstractProductT>			\
	{																				\
	public:																			\
		ConcreteProductT##Factory()													\
		{																			\
			add(*this);																\
		}																			\
		~ConcreteProductT##Factory()												\
		{																			\
			remove(*this);															\
		}																			\
		virtual const std::string className() const override						\
		{																			\
			return (#ConcreteProductT);												\
		}																			\
		virtual gsl::owner<AbstractProductT*> create() override									\
		{																			\
			return new ConcreteProductT();											\
		}																			\
	};
}

#include "Factory.inl"
