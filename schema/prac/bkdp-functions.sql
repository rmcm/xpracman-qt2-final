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
 

 
-- Revision Log
  
-- ------------------------------------------------------------
-- Function to insert a new record with dummy bkdp_code
-- ------------------------------------------------------------

drop function bkdp_insert_row();
create function bkdp_insert_row()
returns integer
as 'DECLARE
    -- NAME TABLE.ATTRIBUTE%TYPE;
    -- NAME TABLE%ROWTYPE;
    tmp_bkdp__sequence bkdp.bkdp__sequence%TYPE;
    tmp_count integer;

    BEGIN

         -- check for paym rows
         select count(*)
         into   tmp_count
         from   paym
         where  paym_bkdp__sequence = 0
         and    (select count(*)
                 from   cred
                 where  cred_paym__sequence = paym.paym__sequence) > 0;

         if ( tmp_count < 1 ) then
            RAISE EXCEPTION ''No valid payments to use in a new batch'';
            return 0;
         end if;

         -- get a bkdp__sequence
         select nextval(''bkdp_bkdp__sequence_seq'')
         into tmp_bkdp__sequence;

         -- add the new row
         insert into bkdp( bkdp__sequence )
                   values( tmp_bkdp__sequence);

         update paym
         set    paym_bkdp__sequence = tmp_bkdp__sequence
         where  paym_bkdp__sequence = 0
         and    (select count(*)
                 from   cred
                 where  cred_paym__sequence = paym.paym__sequence) > 0;

         update bkdp
         set    bkdp_desc = ''''
         where  bkdp__sequence = tmp_bkdp__sequence;
         

    return tmp_bkdp__sequence;
    END;'
  LANGUAGE 'plpgsql';
