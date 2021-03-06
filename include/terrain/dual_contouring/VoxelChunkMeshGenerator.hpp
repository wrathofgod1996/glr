#ifndef DUALCONTOURINGVOXELCHUNKMESHGENERATOR_H_
#define DUALCONTOURINGVOXELCHUNKMESHGENERATOR_H_

#include <vector>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "../TerrainSettings.hpp"
#include "../IVoxelChunkMeshGenerator.hpp"
#include "../IFieldFunction.hpp"

#include "../VoxelChunk.hpp"

namespace glr
{
namespace terrain
{
namespace dual_contouring
{

namespace glmd = glm::detail;

class VoxelChunkMeshGenerator : public IVoxelChunkMeshGenerator
{
public:
	VoxelChunkMeshGenerator(TerrainSettings settings = TerrainSettings());
	VoxelChunkMeshGenerator(IFieldFunction* fieldFunction, TerrainSettings settings = TerrainSettings());
	virtual ~VoxelChunkMeshGenerator();

	/**
	 * Will generate a mesh of the provided VoxelChunk, and put the data in the provided vectors (vertices, normals, and textureBlendingValues).  This function
	 * will use the Dual Contouring algorithm to accomplish the smoothing of the density field data.
	 */
	virtual void generateMesh(VoxelChunk& chunk, glm::detail::int32 length, glm::detail::int32 width, glm::detail::int32 height, std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& normals, std::vector<glm::vec4>& textureBlendingValues) const;
	
private:
	IFieldFunction* fieldFunction_;
	
	struct Point
	{
		glmd::float32 density;
		glm::vec3 pos;
	};
	
	struct Block
	{
		Point points[2][2][2];
		glmd::int32 index;
		glm::vec3 meshPoint;
		glm::vec3 meshPointNormal;
	};
	
	typedef std::vector< std::vector< std::vector<Block> > > Blocks;
	
	/**
	 * Determines whether the points provided define a a fully solid space or a totally empty space.
	 * 
	 * @return True if space is totally empty or solid; false otherwise.
	 */
	bool isEmptyOrSolid(const Points& points) const;
	
	/**
	 * Get noise at the point x, y, z on the grid with the given coordinates.
	 */
	glmd::float32 getInterpolatedNoise(const Points& points, const glm::ivec3& gridCoords, const glm::ivec3& dimensions, glmd::float32 x, glmd::float32 y, glmd::float32 z) const;
	
	/**
	 * Calculate the normal for the provided point.
	 */
	glm::vec3 calculateNormal(const glm::vec3& point, const glm::ivec3& gridCoords, const glm::ivec3& dimensions, const Points& densityValues) const;
	
	/**
	 * Find the intersection along the x-axis where the line segment between p0 and p1 would intersect with the isosurface.
	 */
	void intersectXAxis(Point& p0, Point& p1, Point& out, const glm::ivec3& gridCoords, const glm::ivec3& dimensions, const Points& densityValues) const;
	
	/**
	 * Find the intersection along the y-axis where the line segment between p0 and p1 would intersect with the isosurface.
	 */
	void intersectYAxis(Point& p0, Point& p1, Point& out, const glm::ivec3& gridCoords, const glm::ivec3& dimensions, const Points& densityValues) const;
	
	/**
	 * Find the intersection along the z-axis where the line segment between p0 and p1 would intersect with the isosurface.
	 */
	void intersectZAxis(Point& p0, Point& p1, Point& out, const glm::ivec3& gridCoords, const glm::ivec3& dimensions, const Points& densityValues) const;
	
	/**
	 * Generate the vertex for the given block at position x, y, z.
	 */
	void generateVertex(Blocks& blocks, glmd::int32 x, glmd::int32 y, glmd::int32 z, const glm::ivec3& gridCoords, const glm::ivec3& dimensions, const Points& densityValues) const;
	
	/**
	 * Determine if the cubes along the xz plane at point y have an intersection.  If a cube does, generate the vertex for that block.
	 */
	void computeCubes(Blocks& blocks, glmd::int32 y, const glm::ivec3& gridCoords, const glm::ivec3& dimensions, const Points& densityValues) const;
	
	/**
	 * Generate the 3 points for a triangle along the y coordinate (will move along the xz plane at point y).  Will generate
	 * up to 3 points per block.
	 */
	void generateTriangles(Blocks& blocks, std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& normals, glmd::int32 y) const;
	
	/**
	 * Set the densities and positions for the blocks, using the provided points (density values) and the grid coordinates.
	 */
	void setDensitiesAndPositions(Blocks& blocks, const Points& points, const glm::ivec3& gridCoords, const glm::ivec3& dimensions) const;
	
	/**
	 * Will resize the provided Blocks 3D vector to the appropriate size.
	 * 
	 * @param blocks
	 */
	void resizeBlocks(Blocks& blocks) const;
	
	
	TerrainSettings settings_;
};

}
}
}

#endif /* DUALCONTOURINGVOXELCHUNKMESHGENERATOR_H_ */
