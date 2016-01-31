/*
 * HarrowElementDecomposer.cpp
 *
 *  Created on: Nov 17, 2015
 *      Author: pham
 */

#include "HarrowElementDecomposer.h"
#include <iostream>


/*
 * Implementation of Octave code (may need work to handle SU(4))
 function [v,w]=gc_factor2(u, hermitian_basis, inner_product_func)
 adjust = -2;
 coord_u = real(special_unitary_to_vector(u, hermitian_basis, inner_product_func));
 coord_u = coord_u .* adjust;
 [theta,phi,r]=cart2sph(coord_u(1), coord_u(2), coord_u(3));
 phi2=pi/2-phi;

 [x1,y1,z1]=sph2cart(theta,-phi2,sqrt(r));
 a=[x1 y1 z1];
 v = vector_to_special_unitary_matrix(a' ./ adjust, hermitian_basis);

 [x2,y2,z2]=sph2cart(pi/2+theta,0,sqrt(r));
 b=[x2 y2 z2];
 w = vector_to_special_unitary_matrix(b' ./ adjust, hermitian_basis);
endfunction
 */

template<typename T>
HarrowElementDecomposer<T>::HarrowElementDecomposer(RealCoordinateCalculatorPtr<T> pRealCoordinateCalculator) {
	m_pCoordinateCalculator = pRealCoordinateCalculator;
}

template<typename T>
void HarrowElementDecomposer<T>::decomposeElement(T element, std::vector<T>& subElements, int nbSubElements) {
	//Purpose: U->[V,W,V',W'] | V*W*V'*W' ~ U
	RealCoordinate<T> uSphCoordinate(element, real_coordinate_t());
	calculateSphereCoord(element, uSphCoordinate);

	RealCoordinate<T> vParitionSphCoordinate(NullPtr, real_coordinate_t());
	calculateFirstPartionSphereCoord(uSphCoordinate, vParitionSphCoordinate);

	RealCoordinate<T> wParitionSphCoordinate(NullPtr, real_coordinate_t());
	calculateSecondPartionSphereCoord(uSphCoordinate, wParitionSphCoordinate);

#if DEBUGGING
	printCartFromSphCoord("U", uSphCoordinate.getCoordinates());
	printCartFromSphCoord("V", vParitionSphCoordinate.getCoordinates());
	printCartFromSphCoord("W", wParitionSphCoordinate.getCoordinates());
#endif

	T vElement;
	calculateElementFromSphereCoord(vParitionSphCoordinate, vElement);
	subElements.push_back(vElement);

	T wElement;
	calculateElementFromSphereCoord(wParitionSphCoordinate, wElement);
	subElements.push_back(wElement);

	T vInverseElement;
	getInverseElement(vElement, vInverseElement);
	subElements.push_back(vInverseElement);

	T wInverseElement;
	getInverseElement(wElement, wInverseElement);
	subElements.push_back(wInverseElement);
}

template<typename T>
void HarrowElementDecomposer<T>::calculateSphereCoord(T element, RealCoordinate<T>& elementSphCoord) {
	RealCoordinatePtr<T> pCartCoordinate = NullPtr;
	m_pCoordinateCalculator->calulateElementCoordinate(element, pCartCoordinate);

	real_coordinate_t sphCoord;
	cart2Sph(pCartCoordinate->getCoordinates(), sphCoord);
	elementSphCoord = RealCoordinate<T>(element, sphCoord);
	_destroy(pCartCoordinate);
}

template<typename T>
void HarrowElementDecomposer<T>::calculateElementFromSphereCoord(RealCoordinate<T>& sphCoord, T& element) {
	real_coordinate_t cartCoord;
	sph2Cart(sphCoord.getCoordinates(), cartCoord);

	RealCoordinate<T> elementCartCoord(NullPtr, cartCoord);
	m_pCoordinateCalculator->calculateElementFromCoordinate(&elementCartCoord);
	element = elementCartCoord.getElement();
}

template<typename T>
void HarrowElementDecomposer<T>::calculateFirstPartionSphereCoord(const RealCoordinate<T>& sphCoord, RealCoordinate<T>& firstPartitionSphCoord) {
	real_coordinate_t uCoord = sphCoord.getCoordinates();
	mreal_t theta = uCoord[0];
	mreal_t phi = uCoord[1];
	mreal_t r = uCoord[2];
	real_coordinate_t vCoord = {theta, phi - M_PI_2, mreal::sqrt(r)};
	firstPartitionSphCoord = RealCoordinate<T>(NullPtr, vCoord);
}

template<typename T>
void HarrowElementDecomposer<T>::calculateSecondPartionSphereCoord(const RealCoordinate<T>& sphCoord, RealCoordinate<T>& secondPartitionSphCoord) {
	real_coordinate_t uCoord = sphCoord.getCoordinates();
	mreal_t theta = uCoord[0];
	mreal_t phi = uCoord[1];
	mreal_t r = uCoord[2];
	real_coordinate_t wCoord = {M_PI_2 + theta, 0, mreal::sqrt(r)};
	secondPartitionSphCoord = RealCoordinate<T>(NullPtr, wCoord);
}

template<typename T>
void HarrowElementDecomposer<T>::cart2Sph(const real_coordinate_t& cartCoord, real_coordinate_t& sphCoord) {
	mreal_t x = cartCoord[0] * -2.0;
	mreal_t y = cartCoord[1] * -2.0;
	mreal_t z = cartCoord[2] * -2.0;

	mreal_t azimuth = mreal::atan2(y, x);
	mreal_t elevation = mreal::atan2(z, mreal::sqrt(x * x + y * y));
	mreal_t r = mreal::sqrt(x * x + y * y + z * z);

	sphCoord = real_coordinate_t(3, 0.0);
	sphCoord[0] = azimuth;
	sphCoord[1] = elevation;
	sphCoord[2] = r;
}

template<typename T>
void HarrowElementDecomposer<T>::sph2Cart(const real_coordinate_t& sphCoord, real_coordinate_t& cartCoord) {
	mreal_t azimuth = sphCoord[0];
	mreal_t elevation = sphCoord[1];
	mreal_t r = sphCoord[2];

	mreal_t x = r * mreal::cos(elevation) * mreal::cos(azimuth);
	mreal_t y = r * mreal::cos(elevation) * mreal::sin(azimuth);
	mreal_t z = r * mreal::sin(elevation);

	cartCoord = real_coordinate_t(3, 0.0);
	cartCoord[0] = x / -2.0;
	cartCoord[1] = y / -2.0;
	cartCoord[2] = z / -2.0;
}

#if DEBUGGING
template<typename T>
void HarrowElementDecomposer<T>::printCartFromSphCoord(std::string title, const real_coordinate_t& sphCoord) {
	real_coordinate_t cartCoord;
	sph2Cart(sphCoord, cartCoord);

	std::cout << title << ":";
	for(unsigned int i = 0; i < cartCoord.size(); i++) {
		std::cout << (cartCoord[i]) << " ";
	}
	std::cout  << "\n-----------\n";
}
#endif
