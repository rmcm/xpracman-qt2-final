-- like_integer.sql
-- -*- SQL[ansi] -*-
-- Copyright (C) 2010 Rex McMaster
-- Author: Rex McMaster rex@mcmaster.id.au
-- Created: Wed May 19 14:32:56 2010

-- This program is free software; you can redistribute it and/or
-- modify it under the terms of the GNU General Public License as
-- published by the Free Software Foundation; either version 2, or (at
-- your option) any later version.

-- This program is distributed in the hope that it will be useful, but
-- WITHOUT ANY WARRANTY; without even the implied warranty of
-- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
-- General Public License for more details.

-- You should have received a copy of the GNU General Public License
-- along with this program; see the file COPYING.  If not, write to
-- the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
-- Boston, MA 02110-1301, USA.

-- left integer as string contains right integer as string
create or replace function like_integer(integer, integer)
       returns boolean language sql as $$

       select $1::text ~* ('^' || $2::text);

$$;

comment on function like_integer(integer, integer) is 'integer begins with integer';


drop operator ~* ( integer, integer);
create operator ~* (
    leftarg = integer,
    rightarg = integer,
    procedure = like_integer,
    commutator = ~*
);

drop operator ~ ( integer, integer);
create operator ~ (
    leftarg = integer,
    rightarg = integer,
    procedure = like_integer,
    commutator = ~
);

-- end like_integer.sql
