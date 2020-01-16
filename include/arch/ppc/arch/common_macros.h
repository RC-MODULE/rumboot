#ifndef COMMON_MACRO_H
#define COMMON_MACRO_H

#include <rumboot/macros.h>

#define MACRO_BEGIN         do {
#define MACRO_END           } while( 0 )

#define STRINGIZE( val... ) __stringify_1( val )

#define __CAT( p1, p2 )     p1##p2
#define CAT( p1, p2 )       __CAT( p1, p2 )


#define APPLY( X, p ) X( p )
#define EXPAND_P1( p1, p2 ) p1
#define EXPAND_P2( p1, p2 ) p2

#define REPEAT_1( X, p )    X( 0, p )
#define REPEAT_2( X, p )    REPEAT_1( X, p )    X( 1, p )
#define REPEAT_3( X, p )    REPEAT_2( X, p )    X( 2, p )
#define REPEAT_4( X, p )    REPEAT_3( X, p )    X( 3, p )
#define REPEAT_5( X, p )    REPEAT_4( X, p )    X( 4, p )
#define REPEAT_6( X, p )    REPEAT_5( X, p )    X( 5, p )
#define REPEAT_7( X, p )    REPEAT_6( X, p )    X( 6, p )
#define REPEAT_8( X, p )    REPEAT_7( X, p )    X( 7, p )
#define REPEAT_9( X, p )    REPEAT_8( X, p )    X( 8, p )
#define REPEAT_10( X, p )   REPEAT_9( X, p )    X( 9, p )
#define REPEAT_11( X, p )   REPEAT_10( X, p )   X( 10, p )
#define REPEAT_12( X, p )   REPEAT_11( X, p )   X( 11, p )
#define REPEAT_13( X, p )   REPEAT_12( X, p )   X( 12, p )
#define REPEAT_14( X, p )   REPEAT_13( X, p )   X( 13, p )
#define REPEAT_15( X, p )   REPEAT_14( X, p )   X( 14, p )
#define REPEAT_16( X, p )   REPEAT_15( X, p )   X( 15, p )
#define REPEAT_17( X, p )   REPEAT_16( X, p )   X( 16, p )
#define REPEAT_18( X, p )   REPEAT_17( X, p )   X( 17, p )
#define REPEAT_19( X, p )   REPEAT_18( X, p )   X( 18, p )
#define REPEAT_20( X, p )   REPEAT_19( X, p )   X( 19, p )
#define REPEAT_21( X, p )   REPEAT_20( X, p )   X( 20, p )
#define REPEAT_22( X, p )   REPEAT_21( X, p )   X( 21, p )
#define REPEAT_23( X, p )   REPEAT_22( X, p )   X( 22, p )
#define REPEAT_24( X, p )   REPEAT_23( X, p )   X( 23, p )
#define REPEAT_25( X, p )   REPEAT_24( X, p )   X( 24, p )
#define REPEAT_26( X, p )   REPEAT_25( X, p )   X( 25, p )
#define REPEAT_27( X, p )   REPEAT_26( X, p )   X( 26, p )
#define REPEAT_28( X, p )   REPEAT_27( X, p )   X( 27, p )
#define REPEAT_29( X, p )   REPEAT_28( X, p )   X( 28, p )
#define REPEAT_30( X, p )   REPEAT_29( X, p )   X( 29, p )
#define REPEAT_31( X, p )   REPEAT_30( X, p )   X( 30, p )
#define REPEAT_32( X, p )   REPEAT_31( X, p )   X( 31, p )
#define REPEAT_33( X, p )   REPEAT_32( X, p )   X( 32, p )
#define REPEAT_34( X, p )   REPEAT_33( X, p )   X( 33, p )
#define REPEAT_35( X, p )   REPEAT_34( X, p )   X( 34, p )
#define REPEAT_36( X, p )   REPEAT_35( X, p )   X( 35, p )
#define REPEAT_37( X, p )   REPEAT_36( X, p )   X( 36, p )
#define REPEAT_38( X, p )   REPEAT_37( X, p )   X( 37, p )
#define REPEAT_39( X, p )   REPEAT_38( X, p )   X( 38, p )
#define REPEAT_40( X, p )   REPEAT_39( X, p )   X( 39, p )
#define REPEAT_41( X, p )   REPEAT_40( X, p )   X( 40, p )
#define REPEAT_42( X, p )   REPEAT_41( X, p )   X( 41, p )
#define REPEAT_43( X, p )   REPEAT_42( X, p )   X( 42, p )
#define REPEAT_44( X, p )   REPEAT_43( X, p )   X( 43, p )
#define REPEAT_45( X, p )   REPEAT_44( X, p )   X( 44, p )
#define REPEAT_46( X, p )   REPEAT_45( X, p )   X( 45, p )
#define REPEAT_47( X, p )   REPEAT_46( X, p )   X( 46, p )
#define REPEAT_48( X, p )   REPEAT_47( X, p )   X( 47, p )
#define REPEAT_49( X, p )   REPEAT_48( X, p )   X( 48, p )
#define REPEAT_50( X, p )   REPEAT_49( X, p )   X( 49, p )
#define REPEAT_51( X, p )   REPEAT_50( X, p )   X( 50, p )
#define REPEAT_52( X, p )   REPEAT_51( X, p )   X( 51, p )
#define REPEAT_53( X, p )   REPEAT_52( X, p )   X( 52, p )
#define REPEAT_54( X, p )   REPEAT_53( X, p )   X( 53, p )
#define REPEAT_55( X, p )   REPEAT_54( X, p )   X( 54, p )
#define REPEAT_56( X, p )   REPEAT_55( X, p )   X( 55, p )
#define REPEAT_57( X, p )   REPEAT_56( X, p )   X( 56, p )
#define REPEAT_58( X, p )   REPEAT_57( X, p )   X( 57, p )
#define REPEAT_59( X, p )   REPEAT_58( X, p )   X( 58, p )
#define REPEAT_60( X, p )   REPEAT_59( X, p )   X( 59, p )
#define REPEAT_61( X, p )   REPEAT_60( X, p )   X( 60, p )
#define REPEAT_62( X, p )   REPEAT_61( X, p )   X( 61, p )
#define REPEAT_63( X, p )   REPEAT_62( X, p )   X( 62, p )
#define REPEAT_64( X, p )   REPEAT_63( X, p )   X( 63, p )
#define REPEAT_65( X, p )   REPEAT_64( X, p )   X( 64, p )
#define REPEAT_66( X, p )   REPEAT_65( X, p )   X( 65, p )
#define REPEAT_67( X, p )   REPEAT_66( X, p )   X( 66, p )
#define REPEAT_68( X, p )   REPEAT_67( X, p )   X( 67, p )
#define REPEAT_69( X, p )   REPEAT_68( X, p )   X( 68, p )
#define REPEAT_70( X, p )   REPEAT_69( X, p )   X( 69, p )
#define REPEAT_71( X, p )   REPEAT_70( X, p )   X( 70, p )
#define REPEAT_72( X, p )   REPEAT_71( X, p )   X( 71, p )
#define REPEAT_73( X, p )   REPEAT_72( X, p )   X( 72, p )
#define REPEAT_74( X, p )   REPEAT_73( X, p )   X( 73, p )
#define REPEAT_75( X, p )   REPEAT_74( X, p )   X( 74, p )
#define REPEAT_76( X, p )   REPEAT_75( X, p )   X( 75, p )
#define REPEAT_77( X, p )   REPEAT_76( X, p )   X( 76, p )
#define REPEAT_78( X, p )   REPEAT_77( X, p )   X( 77, p )
#define REPEAT_79( X, p )   REPEAT_78( X, p )   X( 78, p )
#define REPEAT_80( X, p )   REPEAT_79( X, p )   X( 79, p )
#define REPEAT_81( X, p )   REPEAT_80( X, p )   X( 80, p )
#define REPEAT_82( X, p )   REPEAT_81( X, p )   X( 81, p )
#define REPEAT_83( X, p )   REPEAT_82( X, p )   X( 82, p )
#define REPEAT_84( X, p )   REPEAT_83( X, p )   X( 83, p )
#define REPEAT_85( X, p )   REPEAT_84( X, p )   X( 84, p )
#define REPEAT_86( X, p )   REPEAT_85( X, p )   X( 85, p )
#define REPEAT_87( X, p )   REPEAT_86( X, p )   X( 86, p )
#define REPEAT_88( X, p )   REPEAT_87( X, p )   X( 87, p )
#define REPEAT_89( X, p )   REPEAT_88( X, p )   X( 88, p )
#define REPEAT_90( X, p )   REPEAT_89( X, p )   X( 89, p )
#define REPEAT_91( X, p )   REPEAT_90( X, p )   X( 90, p )
#define REPEAT_92( X, p )   REPEAT_91( X, p )   X( 91, p )
#define REPEAT_93( X, p )   REPEAT_92( X, p )   X( 92, p )
#define REPEAT_94( X, p )   REPEAT_93( X, p )   X( 93, p )
#define REPEAT_95( X, p )   REPEAT_94( X, p )   X( 94, p )
#define REPEAT_96( X, p )   REPEAT_95( X, p )   X( 95, p )
#define REPEAT_97( X, p )   REPEAT_96( X, p )   X( 96, p )
#define REPEAT_98( X, p )   REPEAT_97( X, p )   X( 97, p )
#define REPEAT_99( X, p )   REPEAT_98( X, p )   X( 98, p )
#define REPEAT_100( X, p )   REPEAT_99( X, p )   X( 99, p )
#define REPEAT_101( X, p )   REPEAT_100( X, p )   X( 100, p )
#define REPEAT_102( X, p )   REPEAT_101( X, p )   X( 101, p )
#define REPEAT_103( X, p )   REPEAT_102( X, p )   X( 102, p )
#define REPEAT_104( X, p )   REPEAT_103( X, p )   X( 103, p )
#define REPEAT_105( X, p )   REPEAT_104( X, p )   X( 104, p )
#define REPEAT_106( X, p )   REPEAT_105( X, p )   X( 105, p )
#define REPEAT_107( X, p )   REPEAT_106( X, p )   X( 106, p )
#define REPEAT_108( X, p )   REPEAT_107( X, p )   X( 107, p )
#define REPEAT_109( X, p )   REPEAT_108( X, p )   X( 108, p )
#define REPEAT_110( X, p )   REPEAT_109( X, p )   X( 109, p )
#define REPEAT_111( X, p )   REPEAT_110( X, p )   X( 110, p )
#define REPEAT_112( X, p )   REPEAT_111( X, p )   X( 111, p )
#define REPEAT_113( X, p )   REPEAT_112( X, p )   X( 112, p )
#define REPEAT_114( X, p )   REPEAT_113( X, p )   X( 113, p )
#define REPEAT_115( X, p )   REPEAT_114( X, p )   X( 114, p )
#define REPEAT_116( X, p )   REPEAT_115( X, p )   X( 115, p )
#define REPEAT_117( X, p )   REPEAT_116( X, p )   X( 116, p )
#define REPEAT_118( X, p )   REPEAT_117( X, p )   X( 117, p )
#define REPEAT_119( X, p )   REPEAT_118( X, p )   X( 118, p )
#define REPEAT_120( X, p )   REPEAT_119( X, p )   X( 119, p )
#define REPEAT_121( X, p )   REPEAT_120( X, p )   X( 120, p )
#define REPEAT_122( X, p )   REPEAT_121( X, p )   X( 121, p )
#define REPEAT_123( X, p )   REPEAT_122( X, p )   X( 122, p )
#define REPEAT_124( X, p )   REPEAT_123( X, p )   X( 123, p )
#define REPEAT_125( X, p )   REPEAT_124( X, p )   X( 124, p )
#define REPEAT_126( X, p )   REPEAT_125( X, p )   X( 125, p )
#define REPEAT_127( X, p )   REPEAT_126( X, p )   X( 126, p )
#define REPEAT_128( X, p )   REPEAT_127( X, p )   X( 127, p )
#define REPEAT( n, X, p )   CAT( REPEAT_, n )( X, p )

#define REPEAT2_1( X, p )   X( 0, p )
#define REPEAT2_2( X, p )   REPEAT2_1( X, p )   X( 1, p )
#define REPEAT2_3( X, p )   REPEAT2_2( X, p )   X( 2, p )
#define REPEAT2_4( X, p )   REPEAT2_3( X, p )   X( 3, p )
#define REPEAT2_5( X, p )   REPEAT2_4( X, p )   X( 4, p )
#define REPEAT2_6( X, p )   REPEAT2_5( X, p )   X( 5, p )
#define REPEAT2_7( X, p )   REPEAT2_6( X, p )   X( 6, p )
#define REPEAT2_8( X, p )   REPEAT2_7( X, p )   X( 7, p )
#define REPEAT2_9( X, p )   REPEAT2_8( X, p )   X( 8, p )
#define REPEAT2_10( X, p )  REPEAT2_9( X, p )   X( 9, p )
#define REPEAT2_11( X, p )  REPEAT2_10( X, p )  X( 10, p )
#define REPEAT2_12( X, p )  REPEAT2_11( X, p )  X( 11, p )
#define REPEAT2_13( X, p )  REPEAT2_12( X, p )  X( 12, p )
#define REPEAT2_14( X, p )  REPEAT2_13( X, p )  X( 13, p )
#define REPEAT2_15( X, p )  REPEAT2_14( X, p )  X( 14, p )
#define REPEAT2_16( X, p )  REPEAT2_15( X, p )  X( 15, p )
#define REPEAT2_17( X, p )  REPEAT2_16( X, p )  X( 16, p )
#define REPEAT2_18( X, p )  REPEAT2_17( X, p )  X( 17, p )
#define REPEAT2_19( X, p )  REPEAT2_18( X, p )  X( 18, p )
#define REPEAT2_20( X, p )  REPEAT2_19( X, p )  X( 19, p )
#define REPEAT2_21( X, p )  REPEAT2_20( X, p )  X( 20, p )
#define REPEAT2_22( X, p )  REPEAT2_21( X, p )  X( 21, p )
#define REPEAT2_23( X, p )  REPEAT2_22( X, p )  X( 22, p )
#define REPEAT2_24( X, p )  REPEAT2_23( X, p )  X( 23, p )
#define REPEAT2_25( X, p )  REPEAT2_24( X, p )  X( 24, p )
#define REPEAT2_26( X, p )  REPEAT2_25( X, p )  X( 25, p )
#define REPEAT2_27( X, p )  REPEAT2_26( X, p )  X( 26, p )
#define REPEAT2_28( X, p )  REPEAT2_27( X, p )  X( 27, p )
#define REPEAT2_29( X, p )  REPEAT2_28( X, p )  X( 28, p )
#define REPEAT2_30( X, p )  REPEAT2_29( X, p )  X( 29, p )
#define REPEAT2_31( X, p )  REPEAT2_30( X, p )  X( 30, p )
#define REPEAT2_32( X, p )  REPEAT2_31( X, p )  X( 31, p )
#define REPEAT2_33( X, p )  REPEAT2_32( X, p )  X( 32, p )
#define REPEAT2_34( X, p )  REPEAT2_33( X, p )  X( 33, p )
#define REPEAT2_35( X, p )  REPEAT2_34( X, p )  X( 34, p )
#define REPEAT2_36( X, p )  REPEAT2_35( X, p )  X( 35, p )
#define REPEAT2_37( X, p )  REPEAT2_36( X, p )  X( 36, p )
#define REPEAT2_38( X, p )  REPEAT2_37( X, p )  X( 37, p )
#define REPEAT2_39( X, p )  REPEAT2_38( X, p )  X( 38, p )
#define REPEAT2_40( X, p )  REPEAT2_39( X, p )  X( 39, p )
#define REPEAT2_41( X, p )  REPEAT2_40( X, p )  X( 40, p )
#define REPEAT2_42( X, p )  REPEAT2_41( X, p )  X( 41, p )
#define REPEAT2_43( X, p )  REPEAT2_42( X, p )  X( 42, p )
#define REPEAT2_44( X, p )  REPEAT2_43( X, p )  X( 43, p )
#define REPEAT2_45( X, p )  REPEAT2_44( X, p )  X( 44, p )
#define REPEAT2_46( X, p )  REPEAT2_45( X, p )  X( 45, p )
#define REPEAT2_47( X, p )  REPEAT2_46( X, p )  X( 46, p )
#define REPEAT2_48( X, p )  REPEAT2_47( X, p )  X( 47, p )
#define REPEAT2_49( X, p )  REPEAT2_48( X, p )  X( 48, p )
#define REPEAT2_50( X, p )  REPEAT2_49( X, p )  X( 49, p )
#define REPEAT2_51( X, p )  REPEAT2_50( X, p )  X( 50, p )
#define REPEAT2_52( X, p )  REPEAT2_51( X, p )  X( 51, p )
#define REPEAT2_53( X, p )  REPEAT2_52( X, p )  X( 52, p )
#define REPEAT2_54( X, p )  REPEAT2_53( X, p )  X( 53, p )
#define REPEAT2_55( X, p )  REPEAT2_54( X, p )  X( 54, p )
#define REPEAT2_56( X, p )  REPEAT2_55( X, p )  X( 55, p )
#define REPEAT2_57( X, p )  REPEAT2_56( X, p )  X( 56, p )
#define REPEAT2_58( X, p )  REPEAT2_57( X, p )  X( 57, p )
#define REPEAT2_59( X, p )  REPEAT2_58( X, p )  X( 58, p )
#define REPEAT2_60( X, p )  REPEAT2_59( X, p )  X( 59, p )
#define REPEAT2_61( X, p )  REPEAT2_60( X, p )  X( 60, p )
#define REPEAT2_62( X, p )  REPEAT2_61( X, p )  X( 61, p )
#define REPEAT2_63( X, p )  REPEAT2_62( X, p )  X( 62, p )
#define REPEAT2_64( X, p )  REPEAT2_63( X, p )  X( 63, p )
#define REPEAT2_65( X, p )   REPEAT2_64( X, p )   X( 64, p )
#define REPEAT2_66( X, p )   REPEAT2_65( X, p )   X( 65, p )
#define REPEAT2_67( X, p )   REPEAT2_66( X, p )   X( 66, p )
#define REPEAT2_68( X, p )   REPEAT2_67( X, p )   X( 67, p )
#define REPEAT2_69( X, p )   REPEAT2_68( X, p )   X( 68, p )
#define REPEAT2_70( X, p )   REPEAT2_69( X, p )   X( 69, p )
#define REPEAT2_71( X, p )   REPEAT2_70( X, p )   X( 70, p )
#define REPEAT2_72( X, p )   REPEAT2_71( X, p )   X( 71, p )
#define REPEAT2_73( X, p )   REPEAT2_72( X, p )   X( 72, p )
#define REPEAT2_74( X, p )   REPEAT2_73( X, p )   X( 73, p )
#define REPEAT2_75( X, p )   REPEAT2_74( X, p )   X( 74, p )
#define REPEAT2_76( X, p )   REPEAT2_75( X, p )   X( 75, p )
#define REPEAT2_77( X, p )   REPEAT2_76( X, p )   X( 76, p )
#define REPEAT2_78( X, p )   REPEAT2_77( X, p )   X( 77, p )
#define REPEAT2_79( X, p )   REPEAT2_78( X, p )   X( 78, p )
#define REPEAT2_80( X, p )   REPEAT2_79( X, p )   X( 79, p )
#define REPEAT2_81( X, p )   REPEAT2_80( X, p )   X( 80, p )
#define REPEAT2_82( X, p )   REPEAT2_81( X, p )   X( 81, p )
#define REPEAT2_83( X, p )   REPEAT2_82( X, p )   X( 82, p )
#define REPEAT2_84( X, p )   REPEAT2_83( X, p )   X( 83, p )
#define REPEAT2_85( X, p )   REPEAT2_84( X, p )   X( 84, p )
#define REPEAT2_86( X, p )   REPEAT2_85( X, p )   X( 85, p )
#define REPEAT2_87( X, p )   REPEAT2_86( X, p )   X( 86, p )
#define REPEAT2_88( X, p )   REPEAT2_87( X, p )   X( 87, p )
#define REPEAT2_89( X, p )   REPEAT2_88( X, p )   X( 88, p )
#define REPEAT2_90( X, p )   REPEAT2_89( X, p )   X( 89, p )
#define REPEAT2_91( X, p )   REPEAT2_90( X, p )   X( 90, p )
#define REPEAT2_92( X, p )   REPEAT2_91( X, p )   X( 91, p )
#define REPEAT2_93( X, p )   REPEAT2_92( X, p )   X( 92, p )
#define REPEAT2_94( X, p )   REPEAT2_93( X, p )   X( 93, p )
#define REPEAT2_95( X, p )   REPEAT2_94( X, p )   X( 94, p )
#define REPEAT2_96( X, p )   REPEAT2_95( X, p )   X( 95, p )
#define REPEAT2_97( X, p )   REPEAT2_96( X, p )   X( 96, p )
#define REPEAT2_98( X, p )   REPEAT2_97( X, p )   X( 97, p )
#define REPEAT2_99( X, p )   REPEAT2_98( X, p )   X( 98, p )
#define REPEAT2_100( X, p )   REPEAT2_99( X, p )   X( 99, p )
#define REPEAT2_101( X, p )   REPEAT2_100( X, p )   X( 100, p )
#define REPEAT2_102( X, p )   REPEAT2_101( X, p )   X( 101, p )
#define REPEAT2_103( X, p )   REPEAT2_102( X, p )   X( 102, p )
#define REPEAT2_104( X, p )   REPEAT2_103( X, p )   X( 103, p )
#define REPEAT2_105( X, p )   REPEAT2_104( X, p )   X( 104, p )
#define REPEAT2_106( X, p )   REPEAT2_105( X, p )   X( 105, p )
#define REPEAT2_107( X, p )   REPEAT2_106( X, p )   X( 106, p )
#define REPEAT2_108( X, p )   REPEAT2_107( X, p )   X( 107, p )
#define REPEAT2_109( X, p )   REPEAT2_108( X, p )   X( 108, p )
#define REPEAT2_110( X, p )   REPEAT2_109( X, p )   X( 109, p )
#define REPEAT2_111( X, p )   REPEAT2_110( X, p )   X( 110, p )
#define REPEAT2_112( X, p )   REPEAT2_111( X, p )   X( 111, p )
#define REPEAT2_113( X, p )   REPEAT2_112( X, p )   X( 112, p )
#define REPEAT2_114( X, p )   REPEAT2_113( X, p )   X( 113, p )
#define REPEAT2_115( X, p )   REPEAT2_114( X, p )   X( 114, p )
#define REPEAT2_116( X, p )   REPEAT2_115( X, p )   X( 115, p )
#define REPEAT2_117( X, p )   REPEAT2_116( X, p )   X( 116, p )
#define REPEAT2_118( X, p )   REPEAT2_117( X, p )   X( 117, p )
#define REPEAT2_119( X, p )   REPEAT2_118( X, p )   X( 118, p )
#define REPEAT2_120( X, p )   REPEAT2_119( X, p )   X( 119, p )
#define REPEAT2_121( X, p )   REPEAT2_120( X, p )   X( 120, p )
#define REPEAT2_122( X, p )   REPEAT2_121( X, p )   X( 121, p )
#define REPEAT2_123( X, p )   REPEAT2_122( X, p )   X( 122, p )
#define REPEAT2_124( X, p )   REPEAT2_123( X, p )   X( 123, p )
#define REPEAT2_125( X, p )   REPEAT2_124( X, p )   X( 124, p )
#define REPEAT2_126( X, p )   REPEAT2_125( X, p )   X( 125, p )
#define REPEAT2_127( X, p )   REPEAT2_126( X, p )   X( 126, p )
#define REPEAT2_128( X, p )   REPEAT2_127( X, p )   X( 127, p )
#define REPEAT2( n, X, p )  CAT( REPEAT2_, n )( X, p )


#define INC_0   1
#define INC_1   2
#define INC_2   3
#define INC_3   4
#define INC_4   5
#define INC_5   6
#define INC_6   7
#define INC_7   8
#define INC_8   9
#define INC_9   10
#define INC_10  11
#define INC_11  12
#define INC_12  13
#define INC_13  14
#define INC_14  15
#define INC_15  16
#define INC_16  17
#define INC_17  18
#define INC_18  19
#define INC_19  20
#define INC_20  21
#define INC_21  22
#define INC_22  23
#define INC_23  24
#define INC_24  25
#define INC_25  26
#define INC_26  27
#define INC_27  28
#define INC_28  29
#define INC_29  30
#define INC_30  31
#define INC_31  32
#define INC_32  33
#define INC_33  34
#define INC_34  35
#define INC_35  36
#define INC_36  37
#define INC_37  38
#define INC_38  39
#define INC_39  40
#define INC_40  41
#define INC_41  42
#define INC_42  43
#define INC_43  44
#define INC_44  45
#define INC_45  46
#define INC_46  47
#define INC_47  48
#define INC_48  49
#define INC_49  50
#define INC_50  51
#define INC_51  52
#define INC_52  53
#define INC_53  54
#define INC_54  55
#define INC_55  56
#define INC_56  57
#define INC_57  58
#define INC_58  59
#define INC_59  60
#define INC_60  61
#define INC_61  62
#define INC_62  63
#define INC_63  64
#define INC( i ) CAT( INC_, i )

#define ADD_0( n )  n
#define ADD_1( n )  INC( n )
#define ADD_2( n )  ADD_1( INC( n ) )
#define ADD_3( n )  ADD_2( INC( n ) )
#define ADD_4( n )  ADD_3( INC( n ) )
#define ADD_5( n )  ADD_4( INC( n ) )
#define ADD_6( n )  ADD_5( INC( n ) )
#define ADD_7( n )  ADD_6( INC( n ) )
#define ADD_8( n )  ADD_7( INC( n ) )
#define ADD_9( n )  ADD_8( INC( n ) )
#define ADD_10( n ) ADD_9( INC( n ) )
#define ADD_11( n ) ADD_10( INC( n ) )
#define ADD_12( n ) ADD_11( INC( n ) )
#define ADD_13( n ) ADD_12( INC( n ) )
#define ADD_14( n ) ADD_13( INC( n ) )
#define ADD_15( n ) ADD_14( INC( n ) )
#define ADD_16( n ) ADD_15( INC( n ) )
#define ADD_17( n ) ADD_16( INC( n ) )
#define ADD_18( n ) ADD_17( INC( n ) )
#define ADD_19( n ) ADD_18( INC( n ) )
#define ADD_20( n ) ADD_19( INC( n ) )
#define ADD_21( n ) ADD_20( INC( n ) )
#define ADD_22( n ) ADD_21( INC( n ) )
#define ADD_23( n ) ADD_22( INC( n ) )
#define ADD_24( n ) ADD_23( INC( n ) )
#define ADD_25( n ) ADD_24( INC( n ) )
#define ADD_26( n ) ADD_25( INC( n ) )
#define ADD_27( n ) ADD_26( INC( n ) )
#define ADD_28( n ) ADD_27( INC( n ) )
#define ADD_29( n ) ADD_28( INC( n ) )
#define ADD_30( n ) ADD_29( INC( n ) )
#define ADD_31( n ) ADD_30( INC( n ) )
#define ADD_32( n ) ADD_31( INC( n ) )
#define ADD_33( n ) ADD_32( INC( n ) )
#define ADD_34( n ) ADD_33( INC( n ) )
#define ADD_35( n ) ADD_34( INC( n ) )
#define ADD_36( n ) ADD_35( INC( n ) )
#define ADD_37( n ) ADD_36( INC( n ) )
#define ADD_38( n ) ADD_37( INC( n ) )
#define ADD_39( n ) ADD_38( INC( n ) )
#define ADD_40( n ) ADD_39( INC( n ) )
#define ADD_41( n ) ADD_40( INC( n ) )
#define ADD_42( n ) ADD_41( INC( n ) )
#define ADD_43( n ) ADD_42( INC( n ) )
#define ADD_44( n ) ADD_43( INC( n ) )
#define ADD_45( n ) ADD_44( INC( n ) )
#define ADD_46( n ) ADD_45( INC( n ) )
#define ADD_47( n ) ADD_46( INC( n ) )
#define ADD_48( n ) ADD_47( INC( n ) )
#define ADD_49( n ) ADD_48( INC( n ) )
#define ADD_50( n ) ADD_49( INC( n ) )
#define ADD_51( n ) ADD_50( INC( n ) )
#define ADD_52( n ) ADD_51( INC( n ) )
#define ADD_53( n ) ADD_52( INC( n ) )
#define ADD_54( n ) ADD_53( INC( n ) )
#define ADD_55( n ) ADD_54( INC( n ) )
#define ADD_56( n ) ADD_55( INC( n ) )
#define ADD_57( n ) ADD_56( INC( n ) )
#define ADD_58( n ) ADD_57( INC( n ) )
#define ADD_59( n ) ADD_58( INC( n ) )
#define ADD_60( n ) ADD_59( INC( n ) )
#define ADD_61( n ) ADD_60( INC( n ) )
#define ADD_62( n ) ADD_61( INC( n ) )
#define ADD_63( n ) ADD_62( INC( n ) )
#define ADD( n, m ) CAT( ADD_, n )( m )

#define DEC_1   0
#define DEC_2   1
#define DEC_3   2
#define DEC_4   3
#define DEC_5   4
#define DEC_6   5
#define DEC_7   6
#define DEC_8   7
#define DEC_9   8
#define DEC_10  9
#define DEC_11  10
#define DEC_12  11
#define DEC_13  12
#define DEC_14  13
#define DEC_15  14
#define DEC_16  15
#define DEC_17  16
#define DEC_18  17
#define DEC_19  18
#define DEC_20  19
#define DEC_21  20
#define DEC_22  21
#define DEC_23  22
#define DEC_24  23
#define DEC_25  24
#define DEC_26  25
#define DEC_27  26
#define DEC_28  27
#define DEC_29  28
#define DEC_30  29
#define DEC_31  30
#define DEC_32  31
#define DEC_33  32
#define DEC_34  33
#define DEC_35  34
#define DEC_36  35
#define DEC_37  36
#define DEC_38  37
#define DEC_39  38
#define DEC_40  39
#define DEC_41  40
#define DEC_42  41
#define DEC_43  42
#define DEC_44  43
#define DEC_45  44
#define DEC_46  45
#define DEC_47  46
#define DEC_48  47
#define DEC_49  48
#define DEC_50  49
#define DEC_51  50
#define DEC_52  51
#define DEC_53  52
#define DEC_54  53
#define DEC_55  54
#define DEC_56  55
#define DEC_57  56
#define DEC_58  57
#define DEC_59  58
#define DEC_60  59
#define DEC_61  60
#define DEC_62  61
#define DEC_63  62
#define DEC_64  63
#define DEC( i ) CAT( DEC_, i )

#define SUB_0( n )  n
#define SUB_1( n )  DEC( n )
#define SUB_2( n )  SUB_1( DEC( n ) )
#define SUB_3( n )  SUB_2( DEC( n ) )
#define SUB_4( n )  SUB_3( DEC( n ) )
#define SUB_5( n )  SUB_4( DEC( n ) )
#define SUB_6( n )  SUB_5( DEC( n ) )
#define SUB_7( n )  SUB_6( DEC( n ) )
#define SUB_8( n )  SUB_7( DEC( n ) )
#define SUB_9( n )  SUB_8( DEC( n ) )
#define SUB_10( n ) SUB_9( DEC( n ) )
#define SUB_11( n ) SUB_10( DEC( n ) )
#define SUB_12( n ) SUB_11( DEC( n ) )
#define SUB_13( n ) SUB_12( DEC( n ) )
#define SUB_14( n ) SUB_13( DEC( n ) )
#define SUB_15( n ) SUB_14( DEC( n ) )
#define SUB_16( n ) SUB_15( DEC( n ) )
#define SUB_17( n ) SUB_16( DEC( n ) )
#define SUB_18( n ) SUB_17( DEC( n ) )
#define SUB_19( n ) SUB_18( DEC( n ) )
#define SUB_20( n ) SUB_19( DEC( n ) )
#define SUB_21( n ) SUB_20( DEC( n ) )
#define SUB_22( n ) SUB_21( DEC( n ) )
#define SUB_23( n ) SUB_22( DEC( n ) )
#define SUB_24( n ) SUB_23( DEC( n ) )
#define SUB_25( n ) SUB_24( DEC( n ) )
#define SUB_26( n ) SUB_25( DEC( n ) )
#define SUB_27( n ) SUB_26( DEC( n ) )
#define SUB_28( n ) SUB_27( DEC( n ) )
#define SUB_29( n ) SUB_28( DEC( n ) )
#define SUB_30( n ) SUB_29( DEC( n ) )
#define SUB_31( n ) SUB_30( DEC( n ) )
#define SUB_32( n ) SUB_31( DEC( n ) )
#define SUB_33( n ) SUB_32( DEC( n ) )
#define SUB_34( n ) SUB_33( DEC( n ) )
#define SUB_35( n ) SUB_34( DEC( n ) )
#define SUB_36( n ) SUB_35( DEC( n ) )
#define SUB_37( n ) SUB_36( DEC( n ) )
#define SUB_38( n ) SUB_37( DEC( n ) )
#define SUB_39( n ) SUB_38( DEC( n ) )
#define SUB_40( n ) SUB_39( DEC( n ) )
#define SUB_41( n ) SUB_40( DEC( n ) )
#define SUB_42( n ) SUB_41( DEC( n ) )
#define SUB_43( n ) SUB_42( DEC( n ) )
#define SUB_44( n ) SUB_43( DEC( n ) )
#define SUB_45( n ) SUB_44( DEC( n ) )
#define SUB_46( n ) SUB_45( DEC( n ) )
#define SUB_47( n ) SUB_46( DEC( n ) )
#define SUB_48( n ) SUB_47( DEC( n ) )
#define SUB_49( n ) SUB_48( DEC( n ) )
#define SUB_50( n ) SUB_49( DEC( n ) )
#define SUB_51( n ) SUB_50( DEC( n ) )
#define SUB_52( n ) SUB_51( DEC( n ) )
#define SUB_53( n ) SUB_52( DEC( n ) )
#define SUB_54( n ) SUB_53( DEC( n ) )
#define SUB_55( n ) SUB_54( DEC( n ) )
#define SUB_56( n ) SUB_55( DEC( n ) )
#define SUB_57( n ) SUB_56( DEC( n ) )
#define SUB_58( n ) SUB_57( DEC( n ) )
#define SUB_59( n ) SUB_58( DEC( n ) )
#define SUB_60( n ) SUB_59( DEC( n ) )
#define SUB_61( n ) SUB_60( DEC( n ) )
#define SUB_62( n ) SUB_61( DEC( n ) )
#define SUB_63( n ) SUB_62( DEC( n ) )
#define SUB( m, n ) CAT( SUB_, n )( m )

#define MUL_0( n )  0
#define MUL_1( n )  n
#define MUL_2( n )  ADD( MUL_1( n ), n )
#define MUL_3( n )  ADD( MUL_2( n ), n )
#define MUL_4( n )  ADD( MUL_3( n ), n )
#define MUL_5( n )  ADD( MUL_4( n ), n )
#define MUL_6( n )  ADD( MUL_5( n ), n )
#define MUL_7( n )  ADD( MUL_6( n ), n )
#define MUL_8( n )  ADD( MUL_7( n ), n )
#define MUL_9( n )  ADD( MUL_8( n ), n )
#define MUL_10( n ) ADD( MUL_9( n ), n )
#define MUL_11( n ) ADD( MUL_10( n ), n )
#define MUL_12( n ) ADD( MUL_11( n ), n )
#define MUL_13( n ) ADD( MUL_12( n ), n )
#define MUL_14( n ) ADD( MUL_13( n ), n )
#define MUL_15( n ) ADD( MUL_14( n ), n )
#define MUL_16( n ) ADD( MUL_15( n ), n )
#define MUL_17( n ) ADD( MUL_16( n ), n )
#define MUL_18( n ) ADD( MUL_17( n ), n )
#define MUL_19( n ) ADD( MUL_18( n ), n )
#define MUL_20( n ) ADD( MUL_19( n ), n )
#define MUL_21( n ) ADD( MUL_20( n ), n )
#define MUL_22( n ) ADD( MUL_21( n ), n )
#define MUL_23( n ) ADD( MUL_22( n ), n )
#define MUL_24( n ) ADD( MUL_23( n ), n )
#define MUL_25( n ) ADD( MUL_24( n ), n )
#define MUL_26( n ) ADD( MUL_25( n ), n )
#define MUL_27( n ) ADD( MUL_26( n ), n )
#define MUL_28( n ) ADD( MUL_27( n ), n )
#define MUL_29( n ) ADD( MUL_28( n ), n )
#define MUL_30( n ) ADD( MUL_29( n ), n )
#define MUL_31( n ) ADD( MUL_30( n ), n )
#define MUL_32( n ) ADD( MUL_31( n ), n )
#define MUL_33( n ) ADD( MUL_32( n ), n )
#define MUL_34( n ) ADD( MUL_33( n ), n )
#define MUL_35( n ) ADD( MUL_34( n ), n )
#define MUL_36( n ) ADD( MUL_35( n ), n )
#define MUL_37( n ) ADD( MUL_36( n ), n )
#define MUL_38( n ) ADD( MUL_37( n ), n )
#define MUL_39( n ) ADD( MUL_38( n ), n )
#define MUL_40( n ) ADD( MUL_39( n ), n )
#define MUL_41( n ) ADD( MUL_40( n ), n )
#define MUL_42( n ) ADD( MUL_41( n ), n )
#define MUL_43( n ) ADD( MUL_42( n ), n )
#define MUL_44( n ) ADD( MUL_43( n ), n )
#define MUL_45( n ) ADD( MUL_44( n ), n )
#define MUL_46( n ) ADD( MUL_45( n ), n )
#define MUL_47( n ) ADD( MUL_46( n ), n )
#define MUL_48( n ) ADD( MUL_47( n ), n )
#define MUL_49( n ) ADD( MUL_48( n ), n )
#define MUL_50( n ) ADD( MUL_49( n ), n )
#define MUL_51( n ) ADD( MUL_50( n ), n )
#define MUL_52( n ) ADD( MUL_51( n ), n )
#define MUL_53( n ) ADD( MUL_52( n ), n )
#define MUL_54( n ) ADD( MUL_53( n ), n )
#define MUL_55( n ) ADD( MUL_54( n ), n )
#define MUL_56( n ) ADD( MUL_55( n ), n )
#define MUL_57( n ) ADD( MUL_56( n ), n )
#define MUL_58( n ) ADD( MUL_57( n ), n )
#define MUL_59( n ) ADD( MUL_58( n ), n )
#define MUL_60( n ) ADD( MUL_59( n ), n )
#define MUL_61( n ) ADD( MUL_60( n ), n )
#define MUL_62( n ) ADD( MUL_61( n ), n )
#define MUL_63( n ) ADD( MUL_62( n ), n )
#define MUL( n, m ) CAT( MUL_, n )( m )

#endif // COMMON_MACRO_H
