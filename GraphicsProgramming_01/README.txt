These are PARTS of the framework i implemented myself. They are useless by themselves. And the only purpose is to demonstrate parts of the code that create visuals as seen on my portfolio (https://jonasdemaeseneer.wordpress.com/featured-two/) just search for "Implementing features in a Game Engine".

1. FurComponent:  This component can be attached to a Gameobject and will then render fur with the texture provided.
                  It renders the model multiple times but changes the Layer variable in the shader (made with hlsl), 
                  thus making it render a new layer. These (e.g. 20) Layers combined give a fur impression.

2. Material:      This is the base class of materials that are made. In a derived class you can specify the .hlsl file
                  and implement shader specific variables.
                  
3. MeshFilter:    This stores models with all their information

4. ModelAnimator: An object of this is stored in each ModelComponent and takes care of animations.

5. ModelComponent:This component can be attached to a Gameobject and will then render the model with the assigned material
                  and with animations.
                  
6. ShadowMapMaterial: This is a derived class from the Material base class
                      But this is not a normal material that I would put on a model component.
                      I use this material in my ShadowMapRenderer to generate a ShadowMap
                      
7. ShadowMapRenderer: This renderer generates a shadowmap, which can then be used in other materials
                      (e.g DiffuseSkinnedShadow_Material) and these materials use the map to draw shadows on themselves.

