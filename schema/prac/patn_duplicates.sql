--     $Id$
--
--   Copyright 2007 X=X Computer Software Trust
--                Kangaroo Ground Australia 3097
--
--
--  This is free software; you can redistribute it and/or modify
--  it under the terms of the GNU General Public License published by
--  the Free Software Foundation; either version 2, or (at your option)
--  any later version.
--
--  This software is distributed in the hope that it will be useful,
--  but WITHOUT ANY WARRANTY; without even the implied warranty of
--  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
--  GNU General Public License for more details.
--
--  You should have received a copy of the GNU General Public License
--  along with this software; see the file COPYING.  If not, write to
--  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
--
--  Report problems and direct all questions to:
--
--      Rex McMaster, rmcm@compsoft.com.au
--
--


-- find duplicates

select p1.patn_psnam, p1.patn_fsnam, p1.patn_dob, p1.patn__sequence, p2.patn__sequence
from   patn p1, patn p2
where  p1.patn_psnam = p2.patn_fsnam
and    p1.patn_fsnam = p2.patn_fsnam
and    p1.patn_dob = p2.patn_dob
and    p1.patn__sequence != p2.patn__sequence;

