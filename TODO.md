 * select output filename  `-o FILENAME`
   * outdir & name `.gltf` & `.bin` after `.bsp`
 * parse `.json` packed lightmap guides (`bsp_tool.extensions.lightmaps`)
 * 1 `gltf.mesh` per `bsp.Model`

### bsp_tool
 * entity models
   * translate to entity origin
 * lightmaps
   * export
   * remap uvs to lightmap page
