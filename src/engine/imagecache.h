/***************************************************************************
 *   Copyright (C) 2005-2006 by the FIFE Team                              *
 *   fife-public@lists.sourceforge.net                                     *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA              *
 ***************************************************************************/
#ifndef IMAGECACHE_H
#define IMAGECACHE_H

#include "singleton.h"
#include "graphix_primitives.h"
#include <vector>
#include <SDL_video.h>

namespace FIFE {

	class ImageCache : public Singleton<ImageCache> {
		public:
			void clearCache();

			/* add a surface to the cache - note that the surface will be converted into
			 * the display-format, and the orignal surface get's freed - so NEVER use
			 * a pointer you give to this function again
			 */
			size_t addImage(SDL_Surface*);
			Image* getImage(size_t index) const;

			size_t getImageCount() const;

		private:

			typedef std::vector<Image*> type_images;
			type_images m_images;

			SINGLEFRIEND(ImageCache);
	};

}//FIFE

#endif
