--
--   Copyright 2008 X=X Computer Software Trust
--                   Kangaroo Ground Australia 3097
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


\set ON_ERROR_STOP
begin;
drop view evnv cascade;
\i evnv-crea.sql
\i evnv_surgical_views.sql
\i evnv_time_views.sql
commit;

