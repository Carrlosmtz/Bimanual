  function targMap = targDataMap(),

  ;%***********************
  ;% Create Parameter Map *
  ;%***********************
      
    nTotData      = 0; %add to this count as we go
    nTotSects     = 6;
    sectIdxOffset = 0;
    
    ;%
    ;% Define dummy sections & preallocate arrays
    ;%
    dumSection.nData = -1;  
    dumSection.data  = [];
    
    dumData.logicalSrcIdx = -1;
    dumData.dtTransOffset = -1;
    
    ;%
    ;% Init/prealloc paramMap
    ;%
    paramMap.nSections           = nTotSects;
    paramMap.sectIdxOffset       = sectIdxOffset;
      paramMap.sections(nTotSects) = dumSection; %prealloc
    paramMap.nTotData            = -1;
    
    ;%
    ;% Auto data (BimanualHD2LfD_2016a_P)
    ;%
      section.nData     = 7;
      section.data(7)  = dumData; %prealloc
      
	  ;% BimanualHD2LfD_2016a_P.StreamServerRecv_default_value
	  section.data(1).logicalSrcIdx = 0;
	  section.data(1).dtTransOffset = 0;
	
	  ;% BimanualHD2LfD_2016a_P.StreamServerSend_default_value
	  section.data(2).logicalSrcIdx = 1;
	  section.data(2).dtTransOffset = 12;
	
	  ;% BimanualHD2LfD_2016a_P.velNone_Value
	  section.data(3).logicalSrcIdx = 2;
	  section.data(3).dtTransOffset = 16;
	
	  ;% BimanualHD2LfD_2016a_P.dirNone_Value
	  section.data(4).logicalSrcIdx = 3;
	  section.data(4).dtTransOffset = 17;
	
	  ;% BimanualHD2LfD_2016a_P.Constant2_Value
	  section.data(5).logicalSrcIdx = 4;
	  section.data(5).dtTransOffset = 20;
	
	  ;% BimanualHD2LfD_2016a_P.Constant3_Value
	  section.data(6).logicalSrcIdx = 5;
	  section.data(6).dtTransOffset = 21;
	
	  ;% BimanualHD2LfD_2016a_P.Constant1_Value
	  section.data(7).logicalSrcIdx = 6;
	  section.data(7).dtTransOffset = 22;
	
      nTotData = nTotData + section.nData;
      paramMap.sections(1) = section;
      clear section
      
      section.nData     = 4;
      section.data(4)  = dumData; %prealloc
      
	  ;% BimanualHD2LfD_2016a_P.StreamServerRecv_SndPriority
	  section.data(1).logicalSrcIdx = 7;
	  section.data(1).dtTransOffset = 0;
	
	  ;% BimanualHD2LfD_2016a_P.StreamServerRecv_RcvPriority
	  section.data(2).logicalSrcIdx = 8;
	  section.data(2).dtTransOffset = 1;
	
	  ;% BimanualHD2LfD_2016a_P.StreamServerSend_SndPriority
	  section.data(3).logicalSrcIdx = 9;
	  section.data(3).dtTransOffset = 2;
	
	  ;% BimanualHD2LfD_2016a_P.StreamServerSend_RcvPriority
	  section.data(4).logicalSrcIdx = 10;
	  section.data(4).dtTransOffset = 3;
	
      nTotData = nTotData + section.nData;
      paramMap.sections(2) = section;
      clear section
      
      section.nData     = 8;
      section.data(8)  = dumData; %prealloc
      
	  ;% BimanualHD2LfD_2016a_P.StreamServerRecv_SndSize
	  section.data(1).logicalSrcIdx = 11;
	  section.data(1).dtTransOffset = 0;
	
	  ;% BimanualHD2LfD_2016a_P.StreamServerRecv_RcvSize
	  section.data(2).logicalSrcIdx = 12;
	  section.data(2).dtTransOffset = 1;
	
	  ;% BimanualHD2LfD_2016a_P.StreamServerRecv_SndFIFO
	  section.data(3).logicalSrcIdx = 13;
	  section.data(3).dtTransOffset = 2;
	
	  ;% BimanualHD2LfD_2016a_P.StreamServerRecv_RcvFIFO
	  section.data(4).logicalSrcIdx = 14;
	  section.data(4).dtTransOffset = 3;
	
	  ;% BimanualHD2LfD_2016a_P.StreamServerSend_SndSize
	  section.data(5).logicalSrcIdx = 15;
	  section.data(5).dtTransOffset = 4;
	
	  ;% BimanualHD2LfD_2016a_P.StreamServerSend_RcvSize
	  section.data(6).logicalSrcIdx = 16;
	  section.data(6).dtTransOffset = 5;
	
	  ;% BimanualHD2LfD_2016a_P.StreamServerSend_SndFIFO
	  section.data(7).logicalSrcIdx = 17;
	  section.data(7).dtTransOffset = 6;
	
	  ;% BimanualHD2LfD_2016a_P.StreamServerSend_RcvFIFO
	  section.data(8).logicalSrcIdx = 18;
	  section.data(8).dtTransOffset = 7;
	
      nTotData = nTotData + section.nData;
      paramMap.sections(3) = section;
      clear section
      
      section.nData     = 4;
      section.data(4)  = dumData; %prealloc
      
	  ;% BimanualHD2LfD_2016a_P.StreamServerRecv_Optimize
	  section.data(1).logicalSrcIdx = 19;
	  section.data(1).dtTransOffset = 0;
	
	  ;% BimanualHD2LfD_2016a_P.StreamServerRecv_Implementation
	  section.data(2).logicalSrcIdx = 20;
	  section.data(2).dtTransOffset = 1;
	
	  ;% BimanualHD2LfD_2016a_P.StreamServerSend_Optimize
	  section.data(3).logicalSrcIdx = 21;
	  section.data(3).dtTransOffset = 2;
	
	  ;% BimanualHD2LfD_2016a_P.StreamServerSend_Implementation
	  section.data(4).logicalSrcIdx = 22;
	  section.data(4).dtTransOffset = 3;
	
      nTotData = nTotData + section.nData;
      paramMap.sections(4) = section;
      clear section
      
      section.nData     = 6;
      section.data(6)  = dumData; %prealloc
      
	  ;% BimanualHD2LfD_2016a_P.MovementOnOff_CurrentSetting
	  section.data(1).logicalSrcIdx = 23;
	  section.data(1).dtTransOffset = 0;
	
	  ;% BimanualHD2LfD_2016a_P.StreamServerRecv_URI
	  section.data(2).logicalSrcIdx = 24;
	  section.data(2).dtTransOffset = 1;
	
	  ;% BimanualHD2LfD_2016a_P.StreamServerRecv_Endian
	  section.data(3).logicalSrcIdx = 25;
	  section.data(3).dtTransOffset = 23;
	
	  ;% BimanualHD2LfD_2016a_P.DirectionOnOff_CurrentSetting
	  section.data(4).logicalSrcIdx = 26;
	  section.data(4).dtTransOffset = 24;
	
	  ;% BimanualHD2LfD_2016a_P.StreamServerSend_URI
	  section.data(5).logicalSrcIdx = 27;
	  section.data(5).dtTransOffset = 25;
	
	  ;% BimanualHD2LfD_2016a_P.StreamServerSend_Endian
	  section.data(6).logicalSrcIdx = 28;
	  section.data(6).dtTransOffset = 47;
	
      nTotData = nTotData + section.nData;
      paramMap.sections(5) = section;
      clear section
      
      section.nData     = 2;
      section.data(2)  = dumData; %prealloc
      
	  ;% BimanualHD2LfD_2016a_P.StreamServerRecv_Active
	  section.data(1).logicalSrcIdx = 29;
	  section.data(1).dtTransOffset = 0;
	
	  ;% BimanualHD2LfD_2016a_P.StreamServerSend_Active
	  section.data(2).logicalSrcIdx = 30;
	  section.data(2).dtTransOffset = 1;
	
      nTotData = nTotData + section.nData;
      paramMap.sections(6) = section;
      clear section
      
    
      ;%
      ;% Non-auto Data (parameter)
      ;%
    

    ;%
    ;% Add final counts to struct.
    ;%
    paramMap.nTotData = nTotData;
    


  ;%**************************
  ;% Create Block Output Map *
  ;%**************************
      
    nTotData      = 0; %add to this count as we go
    nTotSects     = 2;
    sectIdxOffset = 0;
    
    ;%
    ;% Define dummy sections & preallocate arrays
    ;%
    dumSection.nData = -1;  
    dumSection.data  = [];
    
    dumData.logicalSrcIdx = -1;
    dumData.dtTransOffset = -1;
    
    ;%
    ;% Init/prealloc sigMap
    ;%
    sigMap.nSections           = nTotSects;
    sigMap.sectIdxOffset       = sectIdxOffset;
      sigMap.sections(nTotSects) = dumSection; %prealloc
    sigMap.nTotData            = -1;
    
    ;%
    ;% Auto data (BimanualHD2LfD_2016a_B)
    ;%
      section.nData     = 7;
      section.data(7)  = dumData; %prealloc
      
	  ;% BimanualHD2LfD_2016a_B.MovementOnOff
	  section.data(1).logicalSrcIdx = 0;
	  section.data(1).dtTransOffset = 0;
	
	  ;% BimanualHD2LfD_2016a_B.StreamServerRecv_o4
	  section.data(2).logicalSrcIdx = 1;
	  section.data(2).dtTransOffset = 1;
	
	  ;% BimanualHD2LfD_2016a_B.Reshape
	  section.data(3).logicalSrcIdx = 2;
	  section.data(3).dtTransOffset = 13;
	
	  ;% BimanualHD2LfD_2016a_B.Reshape1
	  section.data(4).logicalSrcIdx = 3;
	  section.data(4).dtTransOffset = 16;
	
	  ;% BimanualHD2LfD_2016a_B.Reshape2
	  section.data(5).logicalSrcIdx = 4;
	  section.data(5).dtTransOffset = 19;
	
	  ;% BimanualHD2LfD_2016a_B.Reshape3
	  section.data(6).logicalSrcIdx = 5;
	  section.data(6).dtTransOffset = 22;
	
	  ;% BimanualHD2LfD_2016a_B.StreamServerSend_o4
	  section.data(7).logicalSrcIdx = 6;
	  section.data(7).dtTransOffset = 25;
	
      nTotData = nTotData + section.nData;
      sigMap.sections(1) = section;
      clear section
      
      section.nData     = 1;
      section.data(1)  = dumData; %prealloc
      
	  ;% BimanualHD2LfD_2016a_B.StreamServerRecv_o5
	  section.data(1).logicalSrcIdx = 7;
	  section.data(1).dtTransOffset = 0;
	
      nTotData = nTotData + section.nData;
      sigMap.sections(2) = section;
      clear section
      
    
      ;%
      ;% Non-auto Data (signal)
      ;%
    

    ;%
    ;% Add final counts to struct.
    ;%
    sigMap.nTotData = nTotData;
    


  ;%*******************
  ;% Create DWork Map *
  ;%*******************
      
    nTotData      = 0; %add to this count as we go
    nTotSects     = 2;
    sectIdxOffset = 2;
    
    ;%
    ;% Define dummy sections & preallocate arrays
    ;%
    dumSection.nData = -1;  
    dumSection.data  = [];
    
    dumData.logicalSrcIdx = -1;
    dumData.dtTransOffset = -1;
    
    ;%
    ;% Init/prealloc dworkMap
    ;%
    dworkMap.nSections           = nTotSects;
    dworkMap.sectIdxOffset       = sectIdxOffset;
      dworkMap.sections(nTotSects) = dumSection; %prealloc
    dworkMap.nTotData            = -1;
    
    ;%
    ;% Auto data (BimanualHD2LfD_2016a_DW)
    ;%
      section.nData     = 2;
      section.data(2)  = dumData; %prealloc
      
	  ;% BimanualHD2LfD_2016a_DW.StreamServerRecv_Stream
	  section.data(1).logicalSrcIdx = 0;
	  section.data(1).dtTransOffset = 0;
	
	  ;% BimanualHD2LfD_2016a_DW.StreamServerSend_Stream
	  section.data(2).logicalSrcIdx = 1;
	  section.data(2).dtTransOffset = 1;
	
      nTotData = nTotData + section.nData;
      dworkMap.sections(1) = section;
      clear section
      
      section.nData     = 3;
      section.data(3)  = dumData; %prealloc
      
	  ;% BimanualHD2LfD_2016a_DW.ControlSpeed_PWORK.LoggedData
	  section.data(1).logicalSrcIdx = 2;
	  section.data(1).dtTransOffset = 0;
	
	  ;% BimanualHD2LfD_2016a_DW.Scope_PWORK.LoggedData
	  section.data(2).logicalSrcIdx = 3;
	  section.data(2).dtTransOffset = 1;
	
	  ;% BimanualHD2LfD_2016a_DW.new1_PWORK.LoggedData
	  section.data(3).logicalSrcIdx = 4;
	  section.data(3).dtTransOffset = 12;
	
      nTotData = nTotData + section.nData;
      dworkMap.sections(2) = section;
      clear section
      
    
      ;%
      ;% Non-auto Data (dwork)
      ;%
    

    ;%
    ;% Add final counts to struct.
    ;%
    dworkMap.nTotData = nTotData;
    


  ;%
  ;% Add individual maps to base struct.
  ;%

  targMap.paramMap  = paramMap;    
  targMap.signalMap = sigMap;
  targMap.dworkMap  = dworkMap;
  
  ;%
  ;% Add checksums to base struct.
  ;%


  targMap.checksum0 = 550266907;
  targMap.checksum1 = 2544446046;
  targMap.checksum2 = 2824691166;
  targMap.checksum3 = 3412559130;

