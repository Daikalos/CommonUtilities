#pragma once

#include <math.h>
#include <vector>
#include <span>

#include <CommonUtilities/Math/Vector2.hpp>
#include <CommonUtilities/Utility/Concepts.hpp>
#include <CommonUtilities/Config.h>

namespace CommonUtilities
{
	template<IsFloatingPointType T = float>
	constexpr void SampleWeightsPolar(Vector2<T> aSamplePoint, std::span<const Vector2<T>> aPoints, std::vector<T>& outWeights)
	{
		static constexpr T kDirScale = T(2);

        outWeights.resize(aPoints.size());

        T totalWeight = T(0);
        T sampleMag = aSamplePoint.Length();
    
        for (std::size_t i = 0; i < aPoints.size(); ++i)
        {      
            Vector2<T>  pointA      = aPoints[i];
            T           lenPointA   = pointA.Length();

            T angleAS = pointA.AngleTo(aSamplePoint);

            T weight = T(1);
            for (std::size_t j = 0; j < aPoints.size(); ++j)
            {
                if (j == i) 
                    continue;
            
                Vector2<T>  pointB      = aPoints[j];
                T           lenPointB   = pointB.Length();
            
                T avgLenAB = (lenPointB + lenPointA) * T(0.5);
           
                T lenAB     = (lenPointB - lenPointA) / avgLenAB;
                T angleAB	= pointA.AngleTo(pointB);

                T lenAS     = (sampleMag - lenPointA) / avgLenAB;
           
                Vector2<T> vecAB;
                vecAB.x = lenAB;
                vecAB.y = angleAB * kDirScale;

                Vector2<T> vecAS;
                vecAS.x = lenAS;
                vecAS.y = angleAS * kDirScale;
            
         	    T lenSqrAB  = vecAB.LengthSqr();
                T newWeight = vecAS.Dot(vecAB) / lenSqrAB;
                newWeight   = T(1) - newWeight;
                newWeight   = std::clamp(newWeight, T(0), T(1));
            
                weight = std::min(newWeight, weight);
            }
        
            outWeights[i]   = weight;
            totalWeight     += weight;
        }

        totalWeight = T(1) / (totalWeight ? totalWeight : T(1));
        for (std::size_t i = 0; i < aPoints.size(); ++i)
        {
            outWeights[i] = outWeights[i] * totalWeight;
        }
	}

    template<IsFloatingPointType T = float>
    constexpr void SampleWeightsCartesian(Vector2<T> aSamplePoint, std::span<const Vector2<T>> aPoints, std::vector<T>& outWeights)
    {
        outWeights.resize(aPoints.size());

        T totalWeight = T(0);

        for (std::size_t i = 0; i < aPoints.size(); ++i)
        {
            Vector2<T> pointA = aPoints[i];
            Vector2<T> vecAS  = aSamplePoint - pointA;
        
            T weight = T(1);

            for (std::size_t j = 0; j < aPoints.size(); ++j)
            {
                if (j == i) 
                    continue;
            
                Vector2<T> pointB   = aPoints[j];
                Vector2<T> vecAB    = pointB - pointA;
            
                T lenSqrAB  = vecAB.LengthSqr();
                T newWeight = vecAS.Dot(vecAB) / lenSqrAB;
                newWeight   = T(1) - newWeight;
                newWeight   = std::clamp(newWeight, T(0), T(1));
            
                weight = std::min(weight, newWeight);
            }
       
            outWeights[i]   = weight;
            totalWeight     += weight;
        }
    
        totalWeight = T(1) / (totalWeight ? totalWeight : T(1));
        for (std::size_t i = 0; i < aPoints.size(); ++i)
        {
            outWeights[i] = outWeights[i] * totalWeight;
        }
    }
}