--  -*- sql -*-
--  
--    $Id$

--  Copyright 2001 X=X Computer Software Trust
--   		  Kangaroo Ground Australia 3097


-- This is free software; you can redistribute it and/or modify
-- it under the terms of the GNU General Public Licensepublished by
-- the Free Software Foundation; either version 2, or (at your option)
-- any later version.
 
-- This software is distributed in the hope that it will be useful,
-- but WITHOUT ANY WARRANTY; without even the implied warranty of
-- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
-- GNU General Public License for more details.
 
-- You should have received a copy of the GNU General Public License
-- along with this software; see the file COPYING.  If not, write to
-- the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 
-- Report problems and direct all questions to:
 
--     Rex McMaster, rmcm@compsoft.com.au
 


-- ------------------------------------------------------------
-- Update the svpf_desc field from serv (prepend to existing)
-- ------------------------------------------------------------

drop function svpf_fixdesc(integer);
create function svpf_fixdesc(integer)
returns integer
as 'DECLARE
    -- NAME TABLE.ATTRIBUTE%TYPE;
    -- NAME TABLE%ROWTYPE;
    tmp_svpf__sequence ALIAS for $1;
    tmp_svpf_desc svpf.svpf_desc%TYPE;

    BEGIN

      select svpf_desc || '' '' || serv_desc
      into   tmp_svpf_desc
      from   svpf, serv
      where  svpf__sequence = tmp_svpf__sequence
      and    svpf_serv_code = serv.serv_code;

      update svpf
      set    svpf_desc = trim(both '' '' from tmp_svpf_desc)
      where  svpf__sequence = tmp_svpf__sequence;

    return tmp_svpf__sequence;
    END;'
  LANGUAGE 'plpgsql';
