// Ce fichier est un composant de la bibliothèque SofaFrontEnd développée 
// par Digital-Trainers SAS. 
//
// L'utilisation de cette bibliothèque est soumise au droit d'auteur et en 
// particulier aux articles L.122-6 et L.122-6-1 du Code de la propriété 
// intellectuelle. Sa détention, sa reproduction ou sa diffusion sont donc 
// soumis à autorisation préalable de la société. 
//
// Pour tout renseignement : contact@digital-trainers.com

#ifndef SFELITE_DATACOPY_H
#define SFELITE_DATACOPY_H

#include "DataTypeTrait.h"

#include <sstream>
#include <iterator>
#include <iostream>
#include <string>
#include <vector>
#include <cstring>

namespace sfe
{

// This function is used internaly in the other SFE libraries to copy values from one supported type to another
template <class InInfo, class OutInfo>
bool copyValue(const void* inPtr, const InInfo* inInfo, void* outPtr, const OutInfo* outInfo)
{
	// Check if types are known
	if (!inInfo->ValidInfo())
	{
		std::cerr << "Input type " << inInfo->type_info()->name() << " not supported" << std::endl;
		return false;
	}
	if (!outInfo->ValidInfo())
	{
		std::cerr << "Output type " << outInfo->type_info()->name() << " not supported" << std::endl;
		return false;
	}

	// Get text value
	if (outInfo->Text() && outInfo->FixedSize() && outInfo->size() == 1) // One output string
	{
		size_t size = inInfo->size(inPtr);
		std::ostringstream out;
		for (size_t i = 0; i < size; ++i)
		{
			if (i) out << " ";
			out << inInfo->getTextValue(inPtr, i);
		}
		outInfo->setTextValue(outPtr, 0, out.str());
		return true;
	}

	// Read text value
	if (inInfo->Text() && inInfo->FixedSize() && inInfo->size() == 1) // One input string
	{
		std::string text = inInfo->getTextValue(inPtr, 0);
		std::istringstream iss(text);
		std::vector<std::string> tokens{ std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>{} };
		int size = tokens.size();
		outInfo->setSize(outPtr, size);
		for (int i = 0; i < size; ++i)
			outInfo->setTextValue(outPtr, i, tokens[i]);
		return true;
	}

	// Compute how many values will be copied
	std::size_t outSize = 1, inSize = 1, copySize = 1, nbElts = 1;
	if (outInfo->FixedSize())
	{
		inSize = inInfo->size(inPtr);
		outSize = outInfo->size();
		if (inSize != outSize)
			std::cerr << "Input has " << inSize << " values while output requires " << outSize << " values." << std::endl;

		copySize = std::min(inSize, outSize);
	}
	else
	{
		inSize = inInfo->size();
		outSize = outInfo->size();
		nbElts = inInfo->size(inPtr) / inSize;
		// Special cases when one type is of size 1, we want to copy every value
		if (inSize == 1)
		{
			copySize = outSize;
			inSize = outSize;
			nbElts /= outSize;
		}
		else if (outSize == 1)
		{
			copySize = inSize;
			outSize = inSize;
		}
		else
		{
			if (inSize != outSize)
				std::cerr << "Input type contains " << inSize << " values per element while output type requires " << outSize << " values." << std::endl;
			copySize = std::min(inSize, outSize);
		}
		outInfo->setSize(outPtr, copySize * nbElts);
	}

	// If possible, use a memcpy
	if (outInfo->SimpleLayout() && inInfo->SimpleLayout() // Continguous in memory
		&& *outInfo->ValueType()->type_info() == *inInfo->ValueType()->type_info() // Same value type
		&& (!outInfo->FixedSize() || inSize == outSize)) // Same size or output is resizeable
	{
		int size = copySize * nbElts * inInfo->byteSize();
		if (size)
			std::memcpy(outInfo->getValuePtr(outPtr), inInfo->getValuePtr(inPtr), size);
		return true;
	}

	// Manual conversion
	if (inInfo->Integer() && outInfo->Integer())
	{
		// integer conversion
		for (size_t l = 0; l<nbElts; ++l)
			for (size_t c = 0; c<copySize; ++c)
				outInfo->setIntegerValue(outPtr, l*outSize + c, inInfo->getIntegerValue(inPtr, l*inSize + c));
	}
	else if ((inInfo->Integer() || inInfo->Scalar()) && (outInfo->Integer() || outInfo->Scalar()))
	{
		// scalar conversion
		for (size_t l = 0; l<nbElts; ++l)
			for (size_t c = 0; c<copySize; ++c)
				outInfo->setScalarValue(outPtr, l*outSize + c, inInfo->getScalarValue(inPtr, l*inSize + c));
	}
	else
	{
		// text conversion
		for (size_t l = 0; l<nbElts; ++l)
			for (size_t c = 0; c<copySize; ++c)
				outInfo->setTextValue(outPtr, l*outSize + c, inInfo->getTextValue(inPtr, l*inSize + c));
	}

	return true;
}

} // namespace sfe

#endif // SFELITE_DATACOPY_H
