//
// 注意: 魔力的地图格式有所不同.
// 假设读入的3X3地图数据顺序为123456789,对应的地图显示:
//     3
//   2   6
// 1   5   9
//   4   8
//     7
// 而不是现在通用的:
//     1
//   2   4
// 3   5   7
//   6   8
//     9
//
// 斜视角地图游标技术:
// 为了绘制的效率和简便性, 先进的斜视角引擎往往采用此类技术.
// 详见: "Introduction to Isometric Engines"
// 这里只给出魔力地图方法:
//      (2,0)
//   (1,0) (2,1)
//(0,0) (1,1) (2,2)
//   (0,1) (1,2)
//      (0,2)
// 直角坐标系向右移一格: x和y值均+1
// 直角坐标系向下移一格:
//   (2,0)		-> 偶数行
//      (2,1)		-> 奇数行(y +1)
//   (1,1)		-> 偶数行(x -1)
//
#pragma once

// 直角坐标系与斜视角坐标系变换

#define DiamondMap_TileRuler_E(n,s) do{	\
	s[0] += Tile_hWidth * (n);		\
	s[1] -= Tile_hHeight * (n);		\
} while(0)
#define DiamondMap_TileRuler_W(n,s) DiamondMap_TileRuler_E(-n,s)

#define DiamondMap_TileRuler_S(n,s) do{	\
	s[0] += Tile_hWidth * (n);		\
	s[1] += Tile_hHeight * (n);		\
} while(0)
#define DiamondMap_TileRuler_N(n,s) DiamondMap_TileRuler_S(-n,s)

// 自动递增
#define DiamondMap_TileWalker_E(n,c,s) do{	\
	DiamondMap_TileRuler_E(n,s);		\
	c[0] += (n);				\
}while(0)
#define DiamondMap_TileWalker_W(n,c,s) DiamondMap_TileWalker_E(-n,c,s)

#define DiamondMap_TileWalker_S(n,c,s) do{	\
	DiamondMap_TileRuler_S(n,s);		\
	c[1] += (n);				\
}while(0)
#define DiamondMap_TileWalker_N(n,c,s) DiamondMap_TileWalker_S(-n,c,s)


#define	DiamondMap_TileRuler_RW(n,c,s) do{		\
	s[0]+=TileWidth;	/* 即直角坐标系右移一格 */	\
	++c[0]; ++c[1];		/* 得到右边斜坐标系格 */	\
}while(0)

#define DiamondMap_TileRuder_LW(n,c,s) do{		\
	s[0]-= TileWidth;	/* 即直角坐标系左移一格 */	\
	--c[0]; --c[1];		/* 得到左边斜坐标系格 */	\
}while(0)

#define DiamondMap_TileRuler_YFix(n)	((!(n & 1)) * (-Tile_hWidth))

// 绘制下一行
#define DiamondMap_TileRuler_FL(n,c,s,t) do{			\
	(n)++;				/* n变为下一行 */		\
	s[0] = DiamondMap_TileRuler_YFix(n) + t;		\
	s[1] += Tile_hHeight;	/* 计算直角坐标系 */		\
	c[n&1] += (n&1) - (!(n&1));	/* 得到下排斜坐标系格 */	\
} while(0)

#define DiamondMap_TileRuler_PL(n,c,s,t) do{			\
	n--;				/* n变为上一行 */		\
	s[0] = DiamondMap_TileRuler_YFix(n) + t;		\
	s[1] -= Tile_hHeight;	/* 计算直角坐标系 */		\
	c[n&1] +=(!(n&1)) - (n&1);	/* 得到上排斜坐标系格 */	\
}while(0)
