/*
Orchid source code is Copyright(c) 2018
Li Wei.
This file is part of Orchid.
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:
- Redistributions of source code must retain the above copyright
notice, this list of conditions and the following disclaimer.
- Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifdef _MSC_VER
#pragma  once
#endif

#ifndef _RAYH_
#define _RAYH_

#include "../math/vector3d.h"

namespace Orchid
{
	class  Ray 
	{
	private:
		Vector3d _origin = { 0.0, 0.0, 0.0 };
		Vector3d _direction = { 0.0, 0.0, 0.0 };
		Vector3d _invdir = { INFTY, INFTY, INFTY };

	public:
		Ray();
		Ray(const Vector3d& origin, const Vector3d& direction);
		Ray(const Ray& ray);
		~Ray();

		Ray& operator=(const Ray& ray);

		inline Vector3d origin()    const { return _origin; }
		inline Vector3d direction() const { return _direction; }
		inline Vector3d invdir()    const { return _invdir; }

	private:
		void calcInvdir();
	};
}

#endif