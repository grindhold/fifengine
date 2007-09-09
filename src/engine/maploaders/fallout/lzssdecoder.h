/***************************************************************************
 *   Copyright (C) 2005 by the FIFE Team                                   *
 *   fife@developer1.cvsdude.com                                           *
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
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef FIFELZSSDECODER_H
#define FIFELZSSDECODER_H

#include "vfs.h"
#include <stdint.h>

namespace FIFE {
	class LZSSDecoder {
		public:
			LZSSDecoder();
			~LZSSDecoder();

			void decode(RawDataPtr input, uint8_t* output, const uint32_t outputsize);

		private:
			uint32_t m_outlen;
			uint32_t m_outindex;

			bool LZSSDecoder::LZSSDecode(RawDataPtr in, long nInputLength, uint8_t* out);

			int getByte(RawDataPtr in, long& inlen) const;
			bool putByte(uint8_t c, uint8_t* out);
			bool FlushBuffer(uint8_t* out);

	};

}

#endif
