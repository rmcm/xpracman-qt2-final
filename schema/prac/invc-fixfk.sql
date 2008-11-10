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
-- Update the patn fk from svpf, and empl,rfdr from patn
-- ------------------------------------------------------------

drop function invc_fixfk(integer);
create function invc_fixfk(integer)
returns integer
as 'DECLARE
    -- NAME TABLE.ATTRIBUTE%TYPE;
    -- NAME TABLE%ROWTYPE;
    tmp_invc__sequence ALIAS for $1;
    tmp_patn__sequence invc.invc_patn__sequence%TYPE;
    rec_patn patn%ROWTYPE;
    count_patients integer;

    BEGIN

      -- There must be a single patient on this invoice.
      select count(distinct svpf_patn__sequence)
      into   count_patients
      from   svpf
      where  svpf_invc__sequence = tmp_invc__sequence;
      if ( count_patients <> 1 ) then
        RAISE EXCEPTION ''Expecting one patient, found <%>'', count_patients;
        return 0;
      end if;

      select  svpf_patn__sequence
      into    tmp_patn__sequence
      from   svpf
      where  svpf_invc__sequence = tmp_invc__sequence;

      if ( not found or tmp_patn__sequence is null ) then
        return 0;
      end if;

      select *
      into   rec_patn
      from   patn
      where  patn__sequence = tmp_patn__sequence;

      if ( not found ) then
         RAISE EXCEPTION ''Patient <%> not found'', tmp_patn__sequence;
         return -1;
      end if;

      -- TODO - all the fks should be conditionally updated (like dbtr)!!
      -- update the debtor if set to default
      update invc
      set    invc_dbtr_code = rec_patn.patn_dbtr_code
      where  invc__sequence = tmp_invc__sequence
      and    invc_dbtr_code = ''-'';

      -- update the remainder of fields
      update invc
      set    invc_patn__sequence = tmp_patn__sequence,
             invc_prov_code = rec_patn.patn_prov_code,
             invc_empl_code = rec_patn.patn_empl_code,
             invc_feet_code = rec_patn.patn_feet_code,
             invc_rfdr_code = rec_patn.patn_rfdr_code,
             invc_rfdr_date = rec_patn.patn_ref_date,
             invc_rfdr_period = rec_patn.patn_ref_period
      where  invc__sequence = tmp_invc__sequence;



    return tmp_patn__sequence;
    END;'
  LANGUAGE 'plpgsql';
